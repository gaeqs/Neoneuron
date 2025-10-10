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

//
// Created by gaeqs on 8/10/25.
//

#ifndef NEONEURON_GPUACTIVITY_H
#define NEONEURON_GPUACTIVITY_H

#include <mindset/mindset.h>
#include <rush/vector/vec.h>
#include <neon/render/model/InstanceData.h>

#include <neoneuron/structure/GID.h>

namespace neoneuron
{

    struct ActivityGPUNeuronData
    {
        mindset::UID datasetId = 0;
        mindset::UID neuronId = 0;
        float dummy1 = 0;
        float dummy2 = 0;
        rush::Vec4f postPosition; // x, y, z, w = valid
    };

    class ActivityGPUNeuron
    {
        GID _gid;
        rush::Vec3f _position;
        neon::InstanceData* _instanceData;
        neon::InstanceData::Instance _instance;
        bool _valid;

      public:
        ActivityGPUNeuron(ActivityGPUNeuron&& other) noexcept;

        ActivityGPUNeuron(const ActivityGPUNeuron& other) = delete;

        ActivityGPUNeuron(neon::InstanceData* instanceData, GID gid, rush::Vec3f position, float value);

        ~ActivityGPUNeuron();

        [[nodiscard]] rush::Vec3f getPosition() const;

        void refreshGPUData() const;

        void updateActivityValue(float value) const;
    };
} // namespace neoneuron
#endif // NEONEURON_GPUACTIVITY_H
