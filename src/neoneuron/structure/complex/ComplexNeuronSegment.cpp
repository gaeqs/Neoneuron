// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "ComplexNeuronSegment.h"

namespace neoneuron
{
    ComplexNeuronSegment::ComplexNeuronSegment(mindset::UID id, mindset::NeuriteType type, const rush::Vec3f& start,
                                               const rush::Vec3f& end, float startRadius, float endRadius,
                                               const std::optional<mindset::UID>& parentId, uint32_t childrenAmount,
                                               uint32_t lod) :
        Identifiable(id),
        _type(type),
        _start(start),
        _end(end),
        _startRadius(startRadius),
        _endRadius(endRadius),
        _parentID(parentId),
        _childrenAmount(childrenAmount),
        _lod(lod)
    {
    }

    mindset::NeuriteType ComplexNeuronSegment::getType() const
    {
        return _type;
    }

    rush::Vec3f ComplexNeuronSegment::getStart() const
    {
        return _start;
    }

    rush::Vec3f ComplexNeuronSegment::getEnd() const
    {
        return _end;
    }

    float ComplexNeuronSegment::getStartRadius() const
    {
        return _startRadius;
    }

    float ComplexNeuronSegment::getEndRadius() const
    {
        return _endRadius;
    }

    std::optional<mindset::UID> ComplexNeuronSegment::getParentId() const
    {
        return _parentID;
    }

    void ComplexNeuronSegment::setType(mindset::NeuriteType type)
    {
        _type = type;
    }

    void ComplexNeuronSegment::setStart(const rush::Vec3f& start)
    {
        _start = start;
    }

    void ComplexNeuronSegment::setEnd(const rush::Vec3f& end)
    {
        _end = end;
    }

    void ComplexNeuronSegment::setStartRadius(float startRadius)
    {
        _startRadius = startRadius;
    }

    void ComplexNeuronSegment::setEndRadius(float endRadius)
    {
        _endRadius = endRadius;
    }

    void ComplexNeuronSegment::setParentId(const mindset::UID& parentId)
    {
        _parentID = parentId;
    }

    uint32_t ComplexNeuronSegment::getChildrenAmount() const
    {
        return _childrenAmount;
    }

    void ComplexNeuronSegment::setChildrenAmount(uint32_t childrenAmount)
    {
        _childrenAmount = childrenAmount;
    }

    void ComplexNeuronSegment::addChildrenToCount()
    {
        ++_childrenAmount;
    }

    uint32_t ComplexNeuronSegment::getLod() const
    {
        return _lod;
    }

    void ComplexNeuronSegment::setLod(uint32_t lod)
    {
        _lod = lod;
    }

    void ComplexNeuronSegment::clearParentId()
    {
        _parentID = {};
    }
} // namespace neoneuron
