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

#ifndef COMPLEXJOINT_H
#define COMPLEXJOINT_H
#include <vector>
#include <mindset/Identifiable.h>

namespace neoneuron
{
    class ComplexNeuron;
    /**
    * Represents a joint of several segments.
    * The UID of this joint must represent the UID
    * of the parent segment.
    */
    class ComplexJoint : public mindset::Identifiable
    {
        std::vector<mindset::UID> _children;
        uint32_t _rotationIndex;

      public:
        ComplexJoint(mindset::UID id, std::vector<mindset::UID> children);

        [[nodiscard]] std::vector<mindset::UID>& getChildren();

        [[nodiscard]] const std::vector<mindset::UID>& getChildren() const;

        [[nodiscard]] uint32_t getRotationIndex() const;

        void computeRotationIndex(ComplexNeuron& neuron);
    };
} // namespace neoneuron

#endif //COMPLEXJOINT_H
