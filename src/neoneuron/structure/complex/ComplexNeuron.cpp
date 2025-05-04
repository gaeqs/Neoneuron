// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "ComplexNeuron.h"

#include <neon/logging/Logger.h>
#include <mindset/util/NeuronTransform.h>

namespace neoneuron
{
    void ComplexNeuron::calculateBoundingBox()
    {
        if (_segments.empty()) {
            _boundingBox = {};
            return;
        }
        rush::Vec3f min = _segments[0].getEnd();
        rush::Vec3f max = _segments[0].getEnd();

        for (size_t i = 1; i < _segments.size(); i++) {
            min = rush::min(min, _segments[i].getEnd());
            max = rush::max(max, _segments[i].getEnd());
        }

        if (_dataset != nullptr) {
            auto prop = _dataset->getProperties().getPropertyUID(mindset::PROPERTY_TRANSFORM);
            auto prototype = getPrototypeNeuron();
            if (prototype != nullptr && prop.has_value()) {
                auto transform = prototype->getProperty<mindset::NeuronTransform>(prop.value());
                if (transform.has_value()) {
                    min = transform->positionToGlobalCoordinates(min);
                    max = transform->positionToGlobalCoordinates(max);
                }
            }
        }
        _boundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuron::calculateJoints()
    {
        _joints.clear();
        _jointsByUID.clear();

        std::map<mindset::UID, ComplexJoint> prototypes;
        for (auto& segment : _segments) {
            if (segment.getParentId().has_value()) {
                auto parent = _segmentsByUID.find(segment.getParentId().value());
                if (parent == _segmentsByUID.end()) {
                    continue;
                }
                // Avoid adding somas!
                if (_segments[parent->second].getType() == mindset::NeuriteType::SOMA) {
                    continue;
                }

                auto it = prototypes.find(segment.getParentId().value());
                if (it == prototypes.end()) {
                    ComplexJoint joint(segment.getParentId().value(), {segment.getUID()});
                    prototypes.insert({segment.getParentId().value(), joint});
                } else {
                    it->second.getChildren().push_back(segment.getUID());
                }
            }
        }

        // Add children attribute and filter 1-1 joints
        _jointsByUID.reserve(prototypes.size());
        _joints.reserve(prototypes.size());
        for (auto& [id, joint] : prototypes) {
            if (auto segment = _segmentsByUID.find(id); segment != _segmentsByUID.end()) {
                _segments[segment->second].setChildrenAmount(joint.getChildren().size());
            }

            if (joint.getChildren().size() < 2) {
                continue;
            }
            joint.computeRotationIndex(*this);
            _jointsByUID.insert({id, _joints.size()});
            _joints.push_back(std::move(joint));
        }
    }

    void ComplexNeuron::calculateSomas()
    {
        _somas.clear();
        _somasByUID.clear();

        // Find somas
        for (auto& segment : _segments) {
            if (segment.getType() == mindset::NeuriteType::SOMA) {
                _somasByUID.insert({segment.getUID(), _somas.size()});
                _somas.emplace_back(segment.getUID());
            }
        }

        // Find children
        for (auto& segment : _segments) {
            if (!segment.getParentId().has_value()) {
                continue;
            }
            auto it = _somasByUID.find(segment.getParentId().value());
            if (it == _somasByUID.end()) {
                continue;
            }
            _somas[it->second].getChildren().push_back(segment.getUID());
        }
    }

    void ComplexNeuron::calculateLOD()
    {
        for (auto& segment : _segments) {
            // We want to start the algorithm from the segment that represents
            // a joint with two or more children or a segment that has no children.
            auto joint = findJoint(segment.getUID());
            if (joint.has_value() && joint.value()->getChildren().size() == 1) {
                continue;
            }
            calculateLODFrom(&segment);
        }
    }

    void ComplexNeuron::calculateLODFrom(ComplexNeuronSegment* segment)
    {
        constexpr size_t MAX_LOD = 7;

        auto distanceSquared = [](const rush::Vec3f& point, const rush::Vec3f& start, const rush::Vec3f& end) {
            rush::Vec3f line = end - start;
            rush::Vec3f toPoint = point - start;

            float t = std::max(toPoint.dot(line) / line.squaredLength(), 0.0f);

            auto projection = start + t * line;
            return (projection - point).squaredLength();
        };

        segment->setLod(0);

        std::vector<ComplexNeuronSegment*> segments;
        segments.push_back(segment);

        // Get all segments
        auto parentId = segment->getParentId();
        while (parentId.has_value()) {
            auto it = _segmentsByUID.find(parentId.value());
            if (it == _segmentsByUID.end()) {
                break;
            }
            auto parent = &_segments[it->second];
            auto parentJoint = findJoint(parentId.value());
            if (parentJoint.has_value() && parentJoint.value()->getChildren().size() > 1) {
                break;
            }
            if (parent->getType() == mindset::NeuriteType::SOMA) {
                break;
            }
            parent->setLod(0);
            segments.push_back(parent);
            parentId = parent->getParentId();
        }

        if (segments.size() < 3) {
            return;
        }

        // Let's use a modified version of the Ramer-Douglas-Peucker algorithm to generate the LOD levels.
        std::vector<std::pair<size_t, float>> significances;
        significances.reserve(segments.size() - 2);
        for (size_t i = 1; i < segments.size() - 1; i++) {
            significances.emplace_back(
                i, distanceSquared(segments[i]->getEnd(), segments[i - 1]->getEnd(), segments[i + 1]->getEnd()));
        }

        std::ranges::sort(significances, [](auto& a, auto& b) { return a.second > b.second; });

        size_t currentLOD = 1;
        size_t threshold = std::max(segments.size() / (MAX_LOD + 1), static_cast<size_t>(1u));
        for (size_t i = 0; i < significances.size(); ++i) {
            if (i > 0 && i % threshold == 0) {
                currentLOD = std::min(currentLOD + 1, MAX_LOD);
            }
            segments[significances[i].first]->setLod(currentLOD);
        }
    }

    ComplexNeuron::ComplexNeuron(ComplexNeuron&& other) noexcept :
        _gid(other._gid),
        _dataset(std::move(other._dataset)),
        _segments(std::move(other._segments)),
        _segmentsByUID(std::move(other._segmentsByUID)),
        _joints(std::move(other._joints)),
        _jointsByUID(std::move(other._jointsByUID)),
        _somas(std::move(other._somas)),
        _somasByUID(std::move(other._somasByUID)),
        _boundingBox(std::move(other._boundingBox))
    {
    }

    ComplexNeuron& ComplexNeuron::operator=(ComplexNeuron&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        _gid = other._gid;
        _dataset = std::move(other._dataset);
        _segments = std::move(other._segments);
        _segmentsByUID = std::move(other._segmentsByUID);
        _joints = std::move(other._joints);
        _jointsByUID = std::move(other._jointsByUID);
        _somas = std::move(other._somas);
        _somasByUID = std::move(other._somasByUID);
        _boundingBox = other._boundingBox;
        return *this;
    }

    ComplexNeuron::ComplexNeuron(GID gid, mindset::Dataset* dataset, mindset::Neuron* prototype) :
        _gid(gid),
        _dataset(dataset)
    {
        auto morphology = prototype->getMorphology();
        if (!morphology.has_value()) {
            neon::error() << "Morphology has not been set";
            return;
        }

        auto lock = dataset->readLock();
        auto& props = dataset->getProperties();
        std::optional<mindset::UID> propType = props.getPropertyUID(mindset::PROPERTY_NEURITE_TYPE);
        std::optional<mindset::UID> propEnd = props.getPropertyUID(mindset::PROPERTY_POSITION);
        std::optional<mindset::UID> propRadius = props.getPropertyUID(mindset::PROPERTY_RADIUS);
        std::optional<mindset::UID> propParent = props.getPropertyUID(mindset::PROPERTY_PARENT);
        if (!propType.has_value() || !propEnd.has_value() || !propRadius.has_value() || !propParent.has_value()) {
            neon::error() << "Cannot load neuron, properties are not found.";
            return;
        }
        lock.unlock();

        auto morphologyLock = morphology.value()->readLock();
        auto neurites = morphology.value()->getNeurites();
        _segments.reserve(neurites.size());
        for (auto segment : neurites) {
            auto type = segment->getProperty<mindset::NeuriteType>(propType.value());
            auto end = segment->getProperty<rush::Vec3f>(propEnd.value());
            auto radius = segment->getProperty<float>(propRadius.value());
            auto parent = segment->getProperty<mindset::UID>(propParent.value());

            if (!type.has_value() || !end.has_value() || !radius.has_value()) {
                neon::error() << "Cannot load section " << segment->getUID() << ". Properties are not found. "
                              << "Type: " << type.has_value() << ", End: " << end.has_value()
                              << ", Radius: " << radius.has_value();
                continue;
            }

            _segments.emplace_back(segment->getUID(), type.value(), end.value(), end.value(), radius.value(),
                                   radius.value(), parent);

            _segmentsByUID.emplace(segment->getUID(), _segmentsByUID.size());
        }

        // Add soma!
        auto soma = morphology.value()->getSoma();
        if (soma.has_value()) {
            auto s = soma.value();
            _segments.emplace_back(s->getUID(), mindset::NeuriteType::SOMA, s->getCenter(), s->getCenter(),
                                   s->getBestMeanRadius(), s->getBestMeanRadius(), std::optional<mindset::UID>());

            _segmentsByUID.emplace(s->getUID(), _segmentsByUID.size());
        }
        morphologyLock.unlock();

        // Load parent data
        for (auto& segment : _segments) {
            if (!segment.getParentId().has_value()) {
                continue;
            }
            auto parentIndex = _segmentsByUID.find(segment.getParentId().value());
            if (parentIndex == _segmentsByUID.end()) {
                neon::error() << "Cannot find parent " << segment.getParentId().value() << ".";
                continue;
            }

            auto& parent = _segments[parentIndex->second];

            segment.setStart(parent.getEnd());
            segment.setStartRadius(parent.getEndRadius());
        }

        recalculateMetadata();
    }

    GID ComplexNeuron::getGID() const
    {
        return _gid;
    }

    mindset::Neuron* ComplexNeuron::getPrototypeNeuron()
    {
        return _dataset->getNeuron(_gid.internalId).value_or(nullptr);
    }

    const mindset::Neuron* ComplexNeuron::getPrototypeNeuron() const
    {
        return _dataset->getNeuron(_gid.internalId).value_or(nullptr);
    }

    mindset::Dataset* ComplexNeuron::getDataset()
    {
        return _dataset;
    }

    const mindset::Dataset* ComplexNeuron::getDataset() const
    {
        return _dataset;
    }

    rush::AABB<3, float> ComplexNeuron::getBoundingBox() const
    {
        return _boundingBox;
    }

    const std::vector<ComplexNeuronSegment>& ComplexNeuron::getSegments() const
    {
        return _segments;
    }

    std::optional<const ComplexNeuronSegment*> ComplexNeuron::findSegment(mindset::UID uid) const
    {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) {
            return {};
        }
        return &_segments[it->second];
    }

    std::optional<size_t> ComplexNeuron::findSegmentIndex(mindset::UID uid) const
    {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) {
            return {};
        }
        return it->second;
    }

    const std::vector<ComplexJoint>& ComplexNeuron::getJoints() const
    {
        return _joints;
    }

    std::optional<const ComplexJoint*> ComplexNeuron::findJoint(mindset::UID uid) const
    {
        auto it = _jointsByUID.find(uid);
        if (it == _jointsByUID.end()) {
            return {};
        }
        return &_joints[it->second];
    }

    std::optional<size_t> ComplexNeuron::findJointIndex(mindset::UID uid) const
    {
        auto it = _jointsByUID.find(uid);
        if (it == _jointsByUID.end()) {
            return {};
        }
        return it->second;
    }

    const std::vector<ComplexSoma>& ComplexNeuron::getSomas() const
    {
        return _somas;
    }

    std::optional<const ComplexSoma*> ComplexNeuron::findSoma(mindset::UID uid) const
    {
        auto it = _somasByUID.find(uid);
        if (it == _somasByUID.end()) {
            return {};
        }
        return &_somas[it->second];
    }

    std::optional<size_t> ComplexNeuron::findSomaIndex(mindset::UID uid) const
    {
        auto it = _somasByUID.find(uid);
        if (it == _somasByUID.end()) {
            return {};
        }
        return it->second;
    }

    void ComplexNeuron::recalculateMetadata()
    {
        calculateBoundingBox();
        calculateJoints();
        calculateSomas();
        calculateLOD();
    }

    void ComplexNeuron::refreshProperty(const std::string& propertyName)
    {
        if (propertyName == mindset::PROPERTY_TRANSFORM) {
            calculateBoundingBox();
        }
    }
} // namespace neoneuron
