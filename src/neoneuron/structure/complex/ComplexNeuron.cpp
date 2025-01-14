//
// Created by gaeqs on 8/10/24.
//

#include "ComplexNeuron.h"

#include <neon/logging/Logger.h>

namespace neoneuron {
    void ComplexNeuron::calculateBoundingBox() {
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

        auto prototype = _prototypeNeuron.lock();
        if (prototype != nullptr) {
            auto transform = prototype->getProperty<NeuronTransform>(PROPERTY_TRANSFORM);
            if (transform.has_value()) {
                auto model = transform.value().getModel();
                min = model * rush::Vec4f(min, 1.0f);
                max = model * rush::Vec4f(max, 1.0f);
            }
        }
        _boundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuron::calculateJoints() {
        _joints.clear();
        _jointsByUID.clear();

        for (auto& segment: _segments) {
            if (segment.getParentId().has_value()) {
                auto parent = _segmentsByUID.find(segment.getParentId().value());
                if (parent == _segmentsByUID.end()) continue;
                // Avoid adding somas!
                if (_segments[parent->second].getType() == SegmentType::SOMA) continue;

                auto it = _jointsByUID.find(segment.getParentId().value());
                if (it == _jointsByUID.end()) {
                    ComplexJoint joint(
                        segment.getParentId().value(),
                        {segment.getId()}
                    );
                    _jointsByUID.insert({segment.getParentId().value(), _joints.size()});
                    _joints.push_back(std::move(joint));
                } else {
                    _joints[it->second].getChildren().push_back(segment.getId());
                }
            }
        }

        for (auto& joint: _joints) {
            joint.computeRotationIndex(*this);
        }
    }

    void ComplexNeuron::calculateSomas() {
        _somas.clear();
        _somasByUID.clear();

        // Find somas
        for (auto& segment: _segments) {
            if (segment.getType() == SegmentType::SOMA) {
                _somasByUID.insert({segment.getId(), _somas.size()});
                _somas.emplace_back(segment.getId());
            }
        }

        // Find children
        for (auto& segment: _segments) {
            if (!segment.getParentId().has_value()) continue;
            auto it = _somasByUID.find(segment.getParentId().value());
            if (it == _somasByUID.end()) continue;
            _somas[it->second].getChildren().push_back(segment.getId());
        }
    }

    void ComplexNeuron::calculateLOD() {
        for (auto& segment: _segments) {
            // We want to start the algorithm from the segment that represents
            // a joint with two or more children or a segment that has no children.
            auto joint = findJoint(segment.getId());
            if (joint.has_value() && joint.value()->getChildren().size() == 1) continue;
            calculateLODFrom(&segment);
        }
    }

    void ComplexNeuron::calculateLODFrom(ComplexNeuronSegment* segment) {
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
            if (it == _segmentsByUID.end()) break;
            auto parent = &_segments[it->second];
            auto parentJoint = findJoint(parentId.value());
            if (parentJoint.has_value() && parentJoint.value()->getChildren().size() > 1) break;
            if (parent->getType() == SegmentType::SOMA) break;
            parent->setLod(0);
            segments.push_back(parent);
            parentId = parent->getParentId();
        }

        if (segments.size() < 3) return;

        // Let's use a modified version of the Ramer-Douglas-Peucker algorithm to generate the LOD levels.
        std::vector<std::pair<size_t, float>> significances;
        significances.reserve(segments.size() - 2);
        for (size_t i = 1; i < segments.size() - 1; i++) {
            significances.emplace_back(
                i,
                distanceSquared(
                    segments[i]->getEnd(),
                    segments[i - 1]->getEnd(),
                    segments[i + 1]->getEnd()
                )
            );
        }

        std::ranges::sort(
            significances,
            [](auto& a, auto& b) {
                return a.second > b.second;
            }
        );

        size_t currentLOD = 1;
        size_t threshold = std::max(segments.size() / (MAX_LOD + 1), static_cast<size_t>(1u));
        for (size_t i = 0; i < significances.size(); ++i) {
            if (i > 0 && i % threshold == 0) {
                currentLOD = std::min(currentLOD + 1, MAX_LOD);
            }
            segments[significances[i].first]->setLod(currentLOD);
        }
    }

    ComplexNeuron::ComplexNeuron(ComplexNeuron&& other) noexcept
        : Identifiable(other.getId()),
          _prototypeNeuron(std::move(other._prototypeNeuron)),
          _segments(std::move(other._segments)),
          _segmentsByUID(std::move(other._segmentsByUID)),
          _joints(std::move(other._joints)),
          _jointsByUID(std::move(other._jointsByUID)),
          _somas(std::move(other._somas)),
          _somasByUID(std::move(other._somasByUID)),
          _boundingBox(std::move(other._boundingBox)) {}

    ComplexNeuron& ComplexNeuron::operator=(ComplexNeuron&& other) noexcept {
        if (this == &other)
            return *this;
        Identifiable::operator =(std::move(other));
        _prototypeNeuron = std::move(other._prototypeNeuron);
        _segments = std::move(other._segments);
        _segmentsByUID = std::move(other._segmentsByUID);
        _joints = std::move(other._joints);
        _jointsByUID = std::move(other._jointsByUID);
        _somas = std::move(other._somas);
        _somasByUID = std::move(other._somasByUID);
        _boundingBox = other._boundingBox;
        return *this;
    }

    ComplexNeuron::ComplexNeuron() : Identifiable(std::numeric_limits<UID>::max()) {}

    ComplexNeuron::ComplexNeuron(std::shared_ptr<PrototypeNeuron> prototype)
        : Identifiable(prototype->getId()),
          _prototypeNeuron(prototype) {
        std::optional<UID> propType = prototype->getPropertyUID(PROPERTY_TYPE);
        std::optional<UID> propEnd = prototype->getPropertyUID(PROPERTY_END);
        std::optional<UID> propRadius = prototype->getPropertyUID(PROPERTY_RADIUS);
        std::optional<UID> propParent = prototype->getPropertyUID(PROPERTY_PARENT);
        if (!propType.has_value() || !propEnd.has_value() || !propRadius.has_value() || !propParent.has_value()) {
            neon::Logger::defaultLogger()->error("Cannot load neuron, properties are not found.");
            return;
        }

        _segments.reserve(prototype->getSegments().size());
        for (auto& segment: prototype->getSegments()) {
            auto type = segment.getProperty<SegmentType>(propType.value());
            auto end = segment.getProperty<rush::Vec3f>(propEnd.value());
            auto radius = segment.getProperty<float>(propRadius.value());
            auto parent = segment.getProperty<int64_t>(propParent.value());

            if (!type.has_value() || !end.has_value() || !radius.has_value() || !parent.has_value()) {
                std::stringstream ss;
                ss << "Cannot load section ";
                ss << segment.getId();
                ss << ". Properties are not found. ";
                ss << "Type: " << type.has_value();
                ss << ", End: " << end.has_value();
                ss << ", Radius: " << radius.has_value();
                ss << ", Parent: " << parent.has_value();
                neon::Logger::defaultLogger()->error(ss.str());
                continue;
            }

            _segments.emplace_back(
                segment.getId(),
                type.value(),
                end.value(),
                end.value(),
                radius.value(),
                radius.value(),
                parent.value() >= 0 ? std::optional(static_cast<UID>(parent.value())) : std::optional<UID>()
            );

            _segmentsByUID.emplace(segment.getId(), _segmentsByUID.size());
        }

        // Load parent data
        for (auto& segment: _segments) {
            if (!segment.getParentId().has_value()) continue;
            auto parentIndex = _segmentsByUID.find(segment.getParentId().value());
            if (parentIndex == _segmentsByUID.end()) {
                std::stringstream ss;
                ss << "Cannot find parent ";
                ss << segment.getParentId().value();
                ss << ".";
                neon::Logger::defaultLogger()->error(ss.str());
                continue;
            }

            auto& parent = _segments[parentIndex->second];

            segment.setStart(parent.getEnd());
            segment.setStartRadius(parent.getEndRadius());
        }

        recalculateMetadata();
    }

    ComplexNeuron::ComplexNeuron(UID uid, const std::vector<ComplexNeuronSegment>& segments)
        : Identifiable(uid),
          _segments(segments) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
        recalculateMetadata();
    }

    ComplexNeuron::ComplexNeuron(UID uid, std::vector<ComplexNeuronSegment>&& segments)
        : Identifiable(uid),
          _segments(std::move(segments)) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
        recalculateMetadata();
    }

    std::optional<PrototypeNeuron*> ComplexNeuron::getPrototypeNeuron() {
        auto prototype = _prototypeNeuron.lock();
        if (prototype == nullptr) return {};
        return prototype.get();
    }

    std::optional<const PrototypeNeuron*> ComplexNeuron::getPrototypeNeuron() const {
        auto prototype = _prototypeNeuron.lock();
        if (prototype == nullptr) return {};
        return prototype.get();
    }

    rush::AABB<3, float> ComplexNeuron::getBoundingBox() const {
        return _boundingBox;
    }

    const std::vector<ComplexNeuronSegment>& ComplexNeuron::getSegments() const {
        return _segments;
    }

    std::optional<const ComplexNeuronSegment*> ComplexNeuron::findSegment(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return &_segments[it->second];
    }

    std::optional<size_t> ComplexNeuron::findSegmentIndex(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return it->second;
    }

    const std::vector<ComplexJoint>& ComplexNeuron::getJoints() const {
        return _joints;
    }

    std::optional<const ComplexJoint*> ComplexNeuron::findJoint(UID uid) const {
        auto it = _jointsByUID.find(uid);
        if (it == _jointsByUID.end()) return {};
        return &_joints[it->second];
    }

    std::optional<size_t> ComplexNeuron::findJointIndex(UID uid) const {
        auto it = _jointsByUID.find(uid);
        if (it == _jointsByUID.end()) return {};
        return it->second;
    }

    const std::vector<ComplexSoma>& ComplexNeuron::getSomas() const {
        return _somas;
    }

    std::optional<const ComplexSoma*> ComplexNeuron::findSoma(UID uid) const {
        auto it = _somasByUID.find(uid);
        if (it == _somasByUID.end()) return {};
        return &_somas[it->second];
    }

    std::optional<size_t> ComplexNeuron::findSomaIndex(UID uid) const {
        auto it = _somasByUID.find(uid);
        if (it == _somasByUID.end()) return {};
        return it->second;
    }

    void ComplexNeuron::recalculateMetadata() {
        calculateBoundingBox();
        calculateJoints();
        calculateSomas();
        calculateLOD();
    }

    void ComplexNeuron::refreshProperty(const std::string& propertyName) {
        if (propertyName == PROPERTY_TRANSFORM) {
            calculateBoundingBox();
        }
    }
}
