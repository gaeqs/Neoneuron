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

#ifndef COMPLEXNEURON_H
#define COMPLEXNEURON_H

#include <vector>
#include <unordered_map>
#include <mindset/Dataset.h>
#include <mindset/Neuron.h>

#include <neoneuron/structure/complex/ComplexNeuronSegment.h>
#include <neoneuron/structure/GID.h>

#include "ComplexJoint.h"
#include "ComplexSoma.h"

namespace neoneuron
{
    class ComplexNeuron
    {
        GID _gid;
        mindset::Dataset* _dataset;
        std::vector<ComplexNeuronSegment> _segments;
        std::unordered_map<mindset::UID, size_t> _segmentsByUID;
        std::vector<ComplexJoint> _joints;
        std::unordered_map<mindset::UID, size_t> _jointsByUID;
        std::vector<ComplexSoma> _somas;
        std::unordered_map<mindset::UID, size_t> _somasByUID;
        rush::AABB<3, float> _boundingBox;

        void calculateBoundingBox();

        void calculateJoints();

        void calculateSomas();

        void calculateLOD();

        void calculateLODFrom(ComplexNeuronSegment* segment);

      public:
        ComplexNeuron(const ComplexNeuron& other) = delete;

        ComplexNeuron(ComplexNeuron&& other) noexcept;

        ComplexNeuron& operator=(ComplexNeuron&& other) noexcept;

        ComplexNeuron(GID gid, mindset::Dataset* dataset, mindset::Neuron* prototype);

        [[nodiscard]] GID getGID() const;

        mindset::Neuron* getPrototypeNeuron();

        const mindset::Neuron* getPrototypeNeuron() const;

        mindset::Dataset* getDataset();

        const mindset::Dataset* getDataset() const;

        [[nodiscard]] rush::AABB<3, float> getBoundingBox() const;

        [[nodiscard]] const std::vector<ComplexNeuronSegment>& getSegments() const;

        [[nodiscard]] std::optional<const ComplexNeuronSegment*> findSegment(mindset::UID uid) const;

        [[nodiscard]] std::optional<size_t> findSegmentIndex(mindset::UID uid) const;

        [[nodiscard]] const std::vector<ComplexJoint>& getJoints() const;

        [[nodiscard]] std::optional<const ComplexJoint*> findJoint(mindset::UID uid) const;

        [[nodiscard]] std::optional<size_t> findJointIndex(mindset::UID uid) const;

        [[nodiscard]] const std::vector<ComplexSoma>& getSomas() const;

        [[nodiscard]] std::optional<const ComplexSoma*> findSoma(mindset::UID uid) const;

        [[nodiscard]] std::optional<size_t> findSomaIndex(mindset::UID uid) const;

        void recalculateMetadata();

        void refreshProperty(const std::string& propertyName);
    };
} // namespace neoneuron

#endif // COMPLEXNEURON_H
