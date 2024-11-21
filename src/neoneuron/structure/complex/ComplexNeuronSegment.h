//
// Created by gaeqs on 8/10/24.
//

#ifndef COMPLEXNEURONSEGMENT_H
#define COMPLEXNEURONSEGMENT_H

#include <optional>

#include <neoneuron/structure/Identifiable.h>
#include <rush/rush.h>

#include <neoneuron/structure/prototype/NeuronProperties.h>

namespace neoneuron {
    class ComplexNeuronSegment : public Identifiable {
        SegmentType _type;
        rush::Vec3f _start;
        rush::Vec3f _end;
        float _startRadius;
        float _endRadius;
        std::optional<UID> _parentID;

    public:
        ComplexNeuronSegment(
            UID id,
            SegmentType type,
            const rush::Vec3f& start,
            const rush::Vec3f& end,
            float startRadius,
            float endRadius,
            const std::optional<UID>& parentId = {}
        );

        [[nodiscard]] SegmentType getType() const;

        void setType(SegmentType type);

        [[nodiscard]] rush::Vec3f getStart() const;

        void setStart(const rush::Vec3f& start);

        [[nodiscard]] rush::Vec3f getEnd() const;

        void setEnd(const rush::Vec3f& end);

        [[nodiscard]] float getStartRadius() const;

        void setStartRadius(float startRadius);

        [[nodiscard]] float getEndRadius() const;

        void setEndRadius(float endRadius);

        [[nodiscard]] std::optional<UID> getParentId() const;

        void setParentId(const UID& parentId);

        void clearParentId();
    };
}


#endif //COMPLEXNEURONSEGMENT_H
