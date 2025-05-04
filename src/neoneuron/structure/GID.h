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

#ifndef NEURONUID_H
#define NEURONUID_H

#include <vector>
#include <mindset/UID.h>

namespace neoneuron
{

    /**
     * A neural element's global identifier.
     * This struct combines the UID of the dataset with the UID
     * of the target element inside the target dataset.
     *
     * For simplicity, dataset UIDs should not be reused
     * when a dataset is destroyed. This is managed automatically
     * by a Repository.
     */
    struct GID
    {
        mindset::UID datasetId;
        mindset::UID internalId;

        GID();

        GID(mindset::UID datasetId, mindset::UID internalId);

        bool operator==(const GID& other) const;

        bool operator!=(const GID& other) const;
    };

} // namespace neoneuron

template<>
struct std::hash<neoneuron::GID>
{
    size_t operator()(const neoneuron::GID& gid) const noexcept
    {
        size_t h1 = std::hash<mindset::UID>{}(gid.datasetId);
        size_t h2 = std::hash<mindset::UID>{}(gid.internalId);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

#endif // NEURONUID_H
