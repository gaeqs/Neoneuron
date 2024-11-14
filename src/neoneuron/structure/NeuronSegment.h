//
// Created by gaeqs on 8/10/24.
//

#ifndef NEURONSEGMENT_H
#define NEURONSEGMENT_H

#include <optional>

#include <neoneuron/structure/Identifiable.h>
#include <rush/rush.h>

#include <neoneuron/structure/prototype/NeuronProperties.h>

namespace neoneuron {
    class NeuronSegment : public Identifiable {
        SegmentType _type;
        rush::Vec3f _start;
        rush::Vec3f _end;
        float _startRadius;
        float _endRadius;
        std::optional<UID> _parentID;

    public:
        NeuronSegment(
            UID id,
            SegmentType type,
            const rush::Vec3f& start,
            const rush::Vec3f& end,
            float startRadius,
            float endRadius,
            const std::optional<UID>& parentId = {}
        );

        [[nodiscard]] SegmentType getType() const;

        [[nodiscard]] rush::Vec3f getStart() const;

        [[nodiscard]] rush::Vec3f getEnd() const;

        [[nodiscard]] float getStartRadius() const;

        [[nodiscard]] float getEndRadius() const;

        [[nodiscard]] std::optional<UID> getParentId() const;
    };
}


#endif //NEURONSEGMENT_H
