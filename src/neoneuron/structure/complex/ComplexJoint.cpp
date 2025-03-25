//
// Created by gaeqs on 22/11/24.
//

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
        for (mnemea::UID& childUID : _children) {
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

    ComplexJoint::ComplexJoint(mnemea::UID id, std::vector<mnemea::UID> children) :
        Identifiable(id),
        _children(std::move(children)),
        _rotationIndex(0)
    {
    }

    std::vector<mnemea::UID>& ComplexJoint::getChildren()
    {
        return _children;
    }

    const std::vector<mnemea::UID>& ComplexJoint::getChildren() const
    {
        return _children;
    }

    uint32_t ComplexJoint::getRotationIndex() const
    {
        return _rotationIndex;
    }
} // namespace neoneuron
