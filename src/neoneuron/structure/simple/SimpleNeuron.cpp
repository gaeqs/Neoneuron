//
// Created by gaeqs on 8/10/24.
//

#include "SimpleNeuron.h"

#include <neon/logging/Logger.h>

namespace neoneuron {
    void SimpleNeuron::calculateBoundingBox() {
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

    SimpleNeuron::SimpleNeuron(SimpleNeuron&& other) noexcept
        : Identifiable(std::move(other)),
          _segments(std::move(other._segments)),
          _segmentsByUID(std::move(other._segmentsByUID)),
          _boundingBox(std::move(other._boundingBox)) {}

    SimpleNeuron& SimpleNeuron::operator=(const SimpleNeuron& other) {
        if (this == &other)
            return *this;
        Identifiable::operator =(other);
        _segments = other._segments;
        _segmentsByUID = other._segmentsByUID;
        _boundingBox = other._boundingBox;
        return *this;
    }

    SimpleNeuron& SimpleNeuron::operator=(SimpleNeuron&& other) noexcept {
        if (this == &other)
            return *this;
        Identifiable::operator =(std::move(other));
        _segments = std::move(other._segments);
        _segmentsByUID = std::move(other._segmentsByUID);
        _boundingBox = other._boundingBox;
        return *this;
    }

    SimpleNeuron::SimpleNeuron() : Identifiable(std::numeric_limits<UID>::max()) {}

    SimpleNeuron::SimpleNeuron(UID uid, const std::vector<SimpleNeuronSegment>& segments)
        : Identifiable(uid),
          _segments(segments) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
        calculateBoundingBox();
    }

    SimpleNeuron::SimpleNeuron(UID uid, std::vector<SimpleNeuronSegment>&& segments)
        : Identifiable(uid),
          _segments(std::move(segments)) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
        calculateBoundingBox();
    }

    rush::AABB<3, float> SimpleNeuron::getBoundingBox() const {
        return _boundingBox;
    }

    const std::vector<SimpleNeuronSegment>& SimpleNeuron::getSegments() const {
        return _segments;
    }

    std::optional<const SimpleNeuronSegment*> SimpleNeuron::findSegment(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return &_segments[it->second];
    }

    std::optional<SimpleNeuronSegment*> SimpleNeuron::findSegment(UID uid) {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return &_segments[it->second];
    }

    std::optional<size_t> SimpleNeuron::findSegmentIndex(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return it->second;
    }

    neon::Result<SimpleNeuron, std::string> SimpleNeuron::fromPrototype(const PrototypeNeuron& prototype) {
        std::optional<UID> propType = prototype.getPropertyUID(PROPERTY_TYPE);
        std::optional<UID> propEnd = prototype.getPropertyUID(PROPERTY_END);
        std::optional<UID> propRadius = prototype.getPropertyUID(PROPERTY_RADIUS);
        std::optional<UID> propParent = prototype.getPropertyUID(PROPERTY_PARENT);
        if (!propType.has_value() || !propEnd.has_value() || !propRadius.has_value() || !propParent.has_value()) {
            return {"Cannot load neuron, properties are not found."};
        }

        std::vector<SimpleNeuronSegment> segments;
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

        return SimpleNeuron(prototype.getId(), segments);
    }
}
