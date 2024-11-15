//
// Created by gaeqs on 8/10/24.
//

#include "Neuron.h"

#include <neon/logging/Logger.h>

namespace neoneuron {
    void Neuron::calculateBoundingBox() {
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

    Neuron::Neuron(Neuron&& other) noexcept
        : Identifiable(std::move(other)),
          _segments(std::move(other._segments)),
          _segmentsByUID(std::move(other._segmentsByUID)),
          _boundingBox(std::move(other._boundingBox)) {}

    Neuron& Neuron::operator=(const Neuron& other) {
        if (this == &other)
            return *this;
        Identifiable::operator =(other);
        _segments = other._segments;
        _segmentsByUID = other._segmentsByUID;
        _boundingBox = other._boundingBox;
        return *this;
    }

    Neuron& Neuron::operator=(Neuron&& other) noexcept {
        if (this == &other)
            return *this;
        Identifiable::operator =(std::move(other));
        _segments = std::move(other._segments);
        _segmentsByUID = std::move(other._segmentsByUID);
        _boundingBox = other._boundingBox;
        return *this;
    }

    Neuron::Neuron() : Identifiable(std::numeric_limits<UID>::max()) {}

    Neuron::Neuron(const PrototypeNeuron& prototype) :
        Identifiable(prototype.getId()) {
        std::optional<UID> propType = prototype.getProperty(PROPERTY_TYPE);
        std::optional<UID> propEnd = prototype.getProperty(PROPERTY_END);
        std::optional<UID> propRadius = prototype.getProperty(PROPERTY_RADIUS);
        std::optional<UID> propParent = prototype.getProperty(PROPERTY_PARENT);
        if (!propType.has_value() || !propEnd.has_value() || !propRadius.has_value() || !propParent.has_value()) {
            neon::Logger::defaultLogger()->error("Cannot load neuron, properties are not found.");
            return;
        }

        _segments.reserve(prototype.getSegments().size());
        for (auto& segment: prototype.getSegments()) {
            auto type = segment.getProperty<SegmentType>(propType.value());
            auto end = segment.getProperty<rush::Vec3f>(propEnd.value());
            auto radius = segment.getProperty<float>(propRadius.value());
            auto parent = segment.getProperty<int64_t>(propParent.value());

            if (!type.has_value() || !end.has_value() || !radius.has_value() || !parent.has_value()) {
                neon::Logger::defaultLogger()->error(neon::MessageBuilder()
                    .print("Cannot load section ")
                    .print(segment.getId())
                    .print(". Properties are not found."));
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
                neon::Logger::defaultLogger()->error(neon::MessageBuilder()
                    .print("Cannot find parent ")
                    .print(segment.getParentId().value())
                    .print("."));
                continue;
            }

            auto& parent = _segments[parentIndex->second];

            segment.setStart(parent.getEnd());
            segment.setStartRadius(parent.getEndRadius());
        }

        calculateBoundingBox();
    }

    Neuron::Neuron(UID uid, const std::vector<NeuronSegment>& segments)
        : Identifiable(uid),
          _segments(segments) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
        calculateBoundingBox();
    }

    Neuron::Neuron(UID uid, std::vector<NeuronSegment>&& segments)
        : Identifiable(uid),
          _segments(std::move(segments)) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
        calculateBoundingBox();
    }

    rush::AABB<3, float> Neuron::getBoundingBox() const {
        return _boundingBox;
    }

    const std::vector<NeuronSegment>& Neuron::getSegments() const {
        return _segments;
    }

    std::optional<const NeuronSegment*> Neuron::findSegment(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return &_segments[it->second];
    }

    std::optional<NeuronSegment*> Neuron::findSegment(UID uid) {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return &_segments[it->second];
    }

    std::optional<size_t> Neuron::findSegmentIndex(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return it->second;
    }
}
