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

#ifndef GPUSYNAPSECREATEINFO_H
#define GPUSYNAPSECREATEINFO_H

#include <neon/Neon.h>
#include <neoneuron/structure/Repository.h>
#include <unordered_map>

namespace neoneuron
{

    struct GPUSynapseCreateInfo
    {
        Repository* repository;
        neon::InstanceData* instanceData;
        std::unordered_map<mindset::UID, mindset::UID> prePositionProperty = {};
        std::unordered_map<mindset::UID, mindset::UID> postPositionProperty = {};

        std::optional<rush::Vec3f> getPrePositionProperty(mindset::UID dataset, const mindset::Synapse* synapse);

        std::optional<rush::Vec3f> getPostPositionProperty(mindset::UID dataset, const mindset::Synapse* synapse);
    };

} // namespace neoneuron

#endif // GPUSYNAPSECREATEINFO_H
