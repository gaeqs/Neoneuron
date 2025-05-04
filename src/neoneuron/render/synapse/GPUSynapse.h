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

#ifndef GPUSYNAPSE_H
#define GPUSYNAPSE_H

#include <mindset/mindset.h>
#include <rush/vector/vec.h>
#include <neon/render/model/InstanceData.h>

#include <neoneuron/structure/GID.h>
#include <neoneuron/render/synapse/GPUSynapseCreateInfo.h>

namespace neoneuron
{

    struct GPUSynapseData
    {
        mindset::UID uid;
        mindset::UID preNeuronId;
        mindset::UID postNeuronId;
        float property;
        rush::Vec4f prePosition;  // x, y, z, w = valid
        rush::Vec4f postPosition; // x, y, z, w = valid
    };

    class GPUSynapse
    {
        GID _gid;
        neon::InstanceData* _instanceData;
        neon::InstanceData::Instance _instance;
        bool _valid;

        std::optional<rush::Vec3f> _prePosition;
        std::optional<rush::Vec3f> _postPosition;

      public:
        GPUSynapse(GPUSynapse&& other) noexcept;

        GPUSynapse(const GPUSynapse& other) = delete;

        GPUSynapse(GPUSynapseCreateInfo& info, GID gid, const mindset::Synapse* synapse);

        ~GPUSynapse();

        [[nodiscard]] std::optional<rush::Vec3f> getPrePosition() const;

        [[nodiscard]] std::optional<rush::Vec3f> getPostPosition() const;

        void refreshGPUData(GPUSynapseCreateInfo& info, const mindset::Synapse* synapse);
    };

} // namespace neoneuron

#endif // GPUSYNAPSE_H
