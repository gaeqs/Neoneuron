//
// Created by gaeqs on 8/10/24.
//

#include "ComplexNeuronSegment.h"

namespace neoneuron {
    ComplexNeuronSegment::ComplexNeuronSegment(
        mnemea::UID id,
        mnemea::NeuriteType type,
        const rush::Vec3f& start,
        const rush::Vec3f& end,
        float startRadius,
        float endRadius,
        const std::optional<mnemea::UID>& parentId,
        uint32_t childrenAmount,
        uint32_t lod
    ): Identifiable(id),
       _type(type),
       _start(start),
       _end(end),
       _startRadius(startRadius),
       _endRadius(endRadius),
       _parentID(parentId),
       _childrenAmount(childrenAmount),
       _lod(lod) {}

    mnemea::NeuriteType ComplexNeuronSegment::getType() const {
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

    std::optional<mnemea::UID> ComplexNeuronSegment::getParentId() const {
        return _parentID;
    }

    void ComplexNeuronSegment::setType(mnemea::NeuriteType type) {
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

    void ComplexNeuronSegment::setParentId(const mnemea::UID& parentId) {
        _parentID = parentId;
    }

    uint32_t ComplexNeuronSegment::getChildrenAmount() const {
        return _childrenAmount;
    }

    void ComplexNeuronSegment::setChildrenAmount(uint32_t childrenAmount) {
        _childrenAmount = childrenAmount;
    }

    void ComplexNeuronSegment::addChildrenToCount() {
        ++_childrenAmount;
    }

    uint32_t ComplexNeuronSegment::getLod() const {
        return _lod;
    }

    void ComplexNeuronSegment::setLod(uint32_t lod) {
        _lod = lod;
    }


    void ComplexNeuronSegment::clearParentId() {
        _parentID = {};
    }
}
