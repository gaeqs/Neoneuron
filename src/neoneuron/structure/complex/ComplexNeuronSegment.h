//
// Created by gaeqs on 8/10/24.
//

#ifndef COMPLEXNEURONSEGMENT_H
#define COMPLEXNEURONSEGMENT_H

#include <optional>

#include <rush/rush.h>
#include <mnemea/DefaultProperties.h>
#include <mnemea/Identifiable.h>

namespace neoneuron {
    class ComplexNeuronSegment : public mnemea::Identifiable {
        mnemea::NeuriteType _type;
        rush::Vec3f _start;
        rush::Vec3f _end;
        float _startRadius;
        float _endRadius;
        std::optional<mnemea::UID> _parentID;
        uint32_t _lod;

    public:
        ComplexNeuronSegment(
            mnemea::UID id,
            mnemea::NeuriteType type,
            const rush::Vec3f& start,
            const rush::Vec3f& end,
            float startRadius,
            float endRadius,
            const std::optional<mnemea::UID>& parentId = {},
            uint32_t lod = 0
        );

        [[nodiscard]] mnemea::NeuriteType getType() const;

        void setType(mnemea::NeuriteType type);

        [[nodiscard]] rush::Vec3f getStart() const;

        void setStart(const rush::Vec3f& start);

        [[nodiscard]] rush::Vec3f getEnd() const;

        void setEnd(const rush::Vec3f& end);

        [[nodiscard]] float getStartRadius() const;

        void setStartRadius(float startRadius);

        [[nodiscard]] float getEndRadius() const;

        void setEndRadius(float endRadius);

        [[nodiscard]] std::optional<mnemea::UID> getParentId() const;

        void setParentId(const mnemea::UID& parentId);

        [[nodiscard]] uint32_t getLod() const;

        void setLod(uint32_t lod);

        void clearParentId();
    };
}


#endif //COMPLEXNEURONSEGMENT_H
