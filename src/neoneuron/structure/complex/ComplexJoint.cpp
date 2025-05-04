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

#include "ComplexJoint.h"

#include <neon/logging/Logger.h>

#include "ComplexNeuron.h"

namespace neoneuron
{
    void ComplexJoint::computeRotationIndex(ComplexNeuron& neuron)
    {
        constexpr uint32_t INDICES = 16;
        constexpr float INDICES_F = static_cast<float>(INDICES);

        if (_children.size() < 2) {
            _rotationIndex = 0;
            return;
        }
        // Fetch the parent and all children.
        auto optParent = neuron.findSegment(getUID());
        if (!optParent.has_value()) {
            return;
        }
        auto parent = optParent.value();
        if (!parent->getParentId().has_value()) {
            return;
        }
        auto optGrandparent = neuron.findSegment(parent->getParentId().value());
        if (!optGrandparent.has_value()) {
            return;
        }
        auto grandparent = optGrandparent.value();
        auto parentDirection = (parent->getEnd() - grandparent->getEnd()).normalized();
        auto tangent = parentDirection.cross(rush::Vec3f(0.0f, 0.0f, -1.0f));

        auto desiredBaseDirection = tangent;
        std::vector<rush::Vec3f> children;
        children.reserve(_children.size());
        for (mindset::UID& childUID : _children) {
            auto optChildren = neuron.findSegment(childUID);
            if (optChildren.has_value()) {
                auto direction = (optChildren.value()->getEnd() - parent->getEnd()).normalized();
                children.push_back(direction);
            }
        }
        if (children.empty()) {
            return;
        }

        auto anglePerChild = 2.0f * std::numbers::pi_v<float> / static_cast<float>(_children.size());
        auto halfAnglePerChild = anglePerChild / 2.0f;

        // Calculate the desired directions.
        std::vector<rush::Vec3f> desiredDirections;
        desiredDirections.reserve(children.size());
        for (size_t i = 0; i < children.size(); i++) {
            auto angle = anglePerChild * static_cast<float>(i) + halfAnglePerChild;
            auto quat = rush::Quatf::angleAxis(angle, parentDirection);
            desiredDirections.push_back(quat * desiredBaseDirection);
        }

        float bestScore = std::numeric_limits<float>::min();
        for (uint32_t i = 0; i < INDICES; ++i) {
            float score = 0.0f;
            auto angle = static_cast<float>(i) * 2.0f * std::numbers::pi_v<float> / INDICES_F;
            auto quat = rush::Quatf::angleAxis(angle, parentDirection);
            for (size_t c = 0; c < children.size(); ++c) {
                auto currentDirection = quat * desiredDirections[c];
                auto localScore = currentDirection.dot(children[c]);
                score += localScore;
            }
            if (score > bestScore) {
                bestScore = score;
                _rotationIndex = i;
            }
        }
    }

    ComplexJoint::ComplexJoint(mindset::UID id, std::vector<mindset::UID> children) :
        Identifiable(id),
        _children(std::move(children)),
        _rotationIndex(0)
    {
    }

    std::vector<mindset::UID>& ComplexJoint::getChildren()
    {
        return _children;
    }

    const std::vector<mindset::UID>& ComplexJoint::getChildren() const
    {
        return _children;
    }

    uint32_t ComplexJoint::getRotationIndex() const
    {
        return _rotationIndex;
    }
} // namespace neoneuron
