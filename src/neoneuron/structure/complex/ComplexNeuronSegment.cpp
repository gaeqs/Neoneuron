//
// Created by gaeqs on 8/10/24.
//

#include "ComplexNeuronSegment.h"

namespace neoneuron {
    ComplexNeuronSegment::ComplexNeuronSegment(
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

    SegmentType ComplexNeuronSegment::getType() const {
        return _type;
    }

    rush::Vec3f ComplexNeuronSegment::getStart() const {
        return _start;
    }

    rush::Vec3f ComplexNeuronSegment::getEnd() const {
        return _end;
    }

    float ComplexNeuronSegment::getStartRadius() const {
        return _startRadius;
    }

    float ComplexNeuronSegment::getEndRadius() const {
        return _endRadius;
    }

    std::optional<UID> ComplexNeuronSegment::getParentId() const {
        return _parentID;
    }

    void ComplexNeuronSegment::setType(SegmentType type) {
        _type = type;
    }

    void ComplexNeuronSegment::setStart(const rush::Vec3f& start) {
        _start = start;
    }

    void ComplexNeuronSegment::setEnd(const rush::Vec3f& end) {
        _end = end;
    }

    void ComplexNeuronSegment::setStartRadius(float startRadius) {
        _startRadius = startRadius;
    }

    void ComplexNeuronSegment::setEndRadius(float endRadius) {
        _endRadius = endRadius;
    }

    void ComplexNeuronSegment::setParentId(const UID& parentId) {
        _parentID = parentId;
    }

    void ComplexNeuronSegment::clearParentId() {
        _parentID = {};
    }
}
