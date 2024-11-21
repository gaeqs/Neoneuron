//
// Created by gaeqs on 8/10/24.
//

#include "SimpleNeuronSegment.h"

namespace neoneuron {
    SimpleNeuronSegment::SimpleNeuronSegment(
        UID id,
        SegmentType type,
        const rush::Vec3f& start,
        const rush::Vec3f& end,
        float startRadius,
        float endRadius,
        const std::optional<UID>& parentId
    ): Identifiable(id),
       _type(type),
       _start(start),
       _end(end),
       _startRadius(startRadius),
       _endRadius(endRadius),
       _parentID(parentId) {}

    SegmentType SimpleNeuronSegment::getType() const {
        return _type;
    }

    rush::Vec3f SimpleNeuronSegment::getStart() const {
        return _start;
    }

    rush::Vec3f SimpleNeuronSegment::getEnd() const {
        return _end;
    }

    float SimpleNeuronSegment::getStartRadius() const {
        return _startRadius;
    }

    float SimpleNeuronSegment::getEndRadius() const {
        return _endRadius;
    }

    std::optional<UID> SimpleNeuronSegment::getParentId() const {
        return _parentID;
    }

    void SimpleNeuronSegment::setType(SegmentType type) {
        _type = type;
    }

    void SimpleNeuronSegment::setStart(const rush::Vec3f& start) {
        _start = start;
    }

    void SimpleNeuronSegment::setEnd(const rush::Vec3f& end) {
        _end = end;
    }

    void SimpleNeuronSegment::setStartRadius(float startRadius) {
        _startRadius = startRadius;
    }

    void SimpleNeuronSegment::setEndRadius(float endRadius) {
        _endRadius = endRadius;
    }

    void SimpleNeuronSegment::setParentId(const UID& parentId) {
        _parentID = parentId;
    }

    void SimpleNeuronSegment::clearParentId() {
        _parentID = {};
    }
}
