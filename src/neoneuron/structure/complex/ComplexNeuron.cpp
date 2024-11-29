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

        _boundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuron::calculateJoints() {
        _joints.clear();
        _jointsByUID.clear();

        for (auto& segment: _segments) {
            if (segment.getParentId().has_value()) {
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

    ComplexNeuron::ComplexNeuron(ComplexNeuron&& other) noexcept
        : Identifiable(std::move(other)),
          _segments(std::move(other._segments)),
          _segmentsByUID(std::move(other._segmentsByUID)),
          _joints(std::move(other._joints)),
          _jointsByUID(std::move(other._jointsByUID)),
          _boundingBox(std::move(other._boundingBox)) {}

    ComplexNeuron& ComplexNeuron::operator=(const ComplexNeuron& other) {
        if (this == &other)
            return *this;
        Identifiable::operator =(other);
        _segments = other._segments;
        _segmentsByUID = other._segmentsByUID;
        _joints = other._joints;
        _jointsByUID = other._jointsByUID;
        _boundingBox = other._boundingBox;
        return *this;
    }

    ComplexNeuron& ComplexNeuron::operator=(ComplexNeuron&& other) noexcept {
        if (this == &other)
            return *this;
        Identifiable::operator =(std::move(other));
        _segments = std::move(other._segments);
        _segmentsByUID = std::move(other._segmentsByUID);
        _joints = std::move(other._joints);
        _jointsByUID = std::move(other._jointsByUID);
        _boundingBox = other._boundingBox;
        return *this;
    }

    ComplexNeuron::ComplexNeuron() : Identifiable(std::numeric_limits<UID>::max()) {}

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
        if (it == _segmentsByUID.end()) return {};
        return &_joints[it->second];
    }

    std::optional<size_t> ComplexNeuron::findJointIndex(UID uid) const {
        auto it = _jointsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return it->second;
    }


    void ComplexNeuron::recalculateMetadata() {
        calculateBoundingBox();
        calculateJoints();
    }

    neon::Result<ComplexNeuron, std::string> ComplexNeuron::fromPrototype(const PrototypeNeuron& prototype) {
        std::optional<UID> propType = prototype.getProperty(PROPERTY_TYPE);
        std::optional<UID> propEnd = prototype.getProperty(PROPERTY_END);
        std::optional<UID> propRadius = prototype.getProperty(PROPERTY_RADIUS);
        std::optional<UID> propParent = prototype.getProperty(PROPERTY_PARENT);
        if (!propType.has_value() || !propEnd.has_value() || !propRadius.has_value() || !propParent.has_value()) {
            return {"Cannot load neuron, properties are not found."};
        }

        std::vector<ComplexNeuronSegment> segments;
        std::unordered_map<UID, size_t> segmentsByUid;


        segments.reserve(prototype.getSegments().size());
        for (auto& segment: prototype.getSegments()) {
            auto type = segment.getProperty<SegmentType>(propType.value());
            auto end = segment.getProperty<rush::Vec3f>(propEnd.value());
            auto radius = segment.getProperty<float>(propRadius.value());
            auto parent = segment.getProperty<int64_t>(propParent.value());

            if (!type.has_value() || !end.has_value() || !radius.has_value() || !parent.has_value()) {
                std::stringstream ss;
                ss << "Cannot load section ";
                ss << segment.getId();
                ss << ". Properties are not found.";
                return ss.str();
            }

            segments.emplace_back(
                segment.getId(),
                type.value(),
                end.value(),
                end.value(),
                radius.value(),
                radius.value(),
                parent.value() >= 0 ? std::optional(static_cast<UID>(parent.value())) : std::optional<UID>()
            );

            segmentsByUid.emplace(segment.getId(), segmentsByUid.size());
        }

        // Load parent data
        for (auto& segment: segments) {
            if (!segment.getParentId().has_value()) continue;
            auto parentIndex = segmentsByUid.find(segment.getParentId().value());
            if (parentIndex == segmentsByUid.end()) {
                std::stringstream ss;
                ss << "Cannot find parent ";
                ss << segment.getParentId().value();
                ss << ".";
                return ss.str();
            }

            auto& parent = segments[parentIndex->second];

            segment.setStart(parent.getEnd());
            segment.setStartRadius(parent.getEndRadius());
        }

        return ComplexNeuron(prototype.getId(), segments);
    }
}
