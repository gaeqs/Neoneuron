//
// Created by gaeqs on 8/10/24.
//

#include "NeuronSegment.h"

namespace neoneuron {
    NeuronSegment::NeuronSegment(
        UID id,
        SegmentType type,
        const rush::Vec3f& start,
        const rush::Vec3f& end,
        float radius,
        const std::optional<UID>& parentId
    ): Identifiable(id),
       _type(type),
       _start(start),
       _end(end),
       _radius(radius),
       _parentID(parentId) {}

    SegmentType NeuronSegment::getType() const {
        return _type;
    }

    rush::Vec3f NeuronSegment::getStart() const {
        return _start;
    }

    rush::Vec3f NeuronSegment::getEnd() const {
        return _end;
    }

    float NeuronSegment::getRadius() const {
        return _radius;
    }

    std::optional<UID> NeuronSegment::getParentId() const {
        return _parentID;
    }
}
