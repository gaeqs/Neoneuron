//
// Created by gaeqs on 8/10/24.
//

#ifndef COMPLEXNEURONSEGMENT_H
#define COMPLEXNEURONSEGMENT_H

#include <optional>

#include <rush/rush.h>
#include <mindset/DefaultProperties.h>
#include <mindset/Identifiable.h>

namespace neoneuron
{
    class ComplexNeuronSegment : public mindset::Identifiable
    {
        mindset::NeuriteType _type;
        rush::Vec3f _start;
        rush::Vec3f _end;
        float _startRadius;
        float _endRadius;
        std::optional<mindset::UID> _parentID;
        uint32_t _childrenAmount;
        uint32_t _lod;

      public:
        ComplexNeuronSegment(mindset::UID id, mindset::NeuriteType type, const rush::Vec3f& start, const rush::Vec3f& end,
                             float startRadius, float endRadius, const std::optional<mindset::UID>& parentId = {},
                             uint32_t childrenAmount = 0, uint32_t lod = 0);

        [[nodiscard]] mindset::NeuriteType getType() const;

        void setType(mindset::NeuriteType type);

        [[nodiscard]] rush::Vec3f getStart() const;

        void setStart(const rush::Vec3f& start);

        [[nodiscard]] rush::Vec3f getEnd() const;

        void setEnd(const rush::Vec3f& end);

        [[nodiscard]] float getStartRadius() const;

        void setStartRadius(float startRadius);

        [[nodiscard]] float getEndRadius() const;

        void setEndRadius(float endRadius);

        [[nodiscard]] std::optional<mindset::UID> getParentId() const;

        void setParentId(const mindset::UID& parentId);

        [[nodiscard]] uint32_t getChildrenAmount() const;

        void setChildrenAmount(uint32_t childrenAmount);

        void addChildrenToCount();

        [[nodiscard]] uint32_t getLod() const;

        void setLod(uint32_t lod);

        void clearParentId();
    };
} // namespace neoneuron

#endif //COMPLEXNEURONSEGMENT_H
