//
// Created by gaeqs on 8/10/24.
//

#ifndef NEURONSEGMENT_H
#define NEURONSEGMENT_H

#include <optional>

#include <neoneuron/structure/Identifiable.h>
#include <rush/rush.h>

namespace neoneuron {
    enum class SegmentType : uint8_t {
        UNDEFINED = 0,
        SOMA = 1,
        AXON = 2,
        BASAL_DENDRITE = 3,
        APICAL_DENDRITE = 4,
        CUSTOM = 5,
        UNSPECIFIED_NEURITE = 6,
        GLIA_PROCESSES = 7
    };

    class NeuronSegment : public Identifiable {
        SegmentType _type;
        rush::Vec3f _start;
        rush::Vec3f _end;
        float _radius;
        std::optional<UID> _parentID;

    public:
        NeuronSegment(
            UID id,
            SegmentType type,
            const rush::Vec3f& start,
            const rush::Vec3f& end,
            float radius,
            const std::optional<UID>& parentId = {}
        );

        [[nodiscard]] SegmentType getType() const;

        [[nodiscard]] rush::Vec3f getStart() const;

        [[nodiscard]] rush::Vec3f getEnd() const;

        [[nodiscard]] float getRadius() const;

        [[nodiscard]] std::optional<UID> getParentId() const;
    };
}


#endif //NEURONSEGMENT_H
