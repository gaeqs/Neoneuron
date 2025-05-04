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

#include "GPUSynapseCreateInfo.h"

namespace neoneuron
{

    std::optional<rush::Vec3f> GPUSynapseCreateInfo::getPrePositionProperty(mindset::UID dataset,
                                                                            const mindset::Synapse* synapse)
    {
        auto it = prePositionProperty.find(dataset);
        if (it == prePositionProperty.end()) {
            // Fetch property
            auto db = repository->getDataset(dataset);
            if (!db.has_value()) {
                return {};
            }
            auto lock = db.value()->getDataset().readLock();
            auto prop = db.value()->getDataset().getProperties().getPropertyUID(mindset::PROPERTY_SYNAPSE_PRE_POSITION);
            if (!prop.has_value()) {
                return {};
            }
            lock.unlock();
            auto [result, ok] = prePositionProperty.insert({dataset, prop.value()});
            if (!ok) {
                return {};
            }
            it = result;
        }

        return synapse->getProperty<rush::Vec3f>(it->second);
    }

    std::optional<rush::Vec3f> GPUSynapseCreateInfo::getPostPositionProperty(mindset::UID dataset,
                                                                             const mindset::Synapse* synapse)
    {
        auto it = postPositionProperty.find(dataset);
        if (it == prePositionProperty.end()) {
            // Fetch property
            auto db = repository->getDataset(dataset);
            if (!db.has_value()) {
                return {};
            }
            auto lock = db.value()->getDataset().readLock();
            auto prop =
                db.value()->getDataset().getProperties().getPropertyUID(mindset::PROPERTY_SYNAPSE_POST_POSITION);
            if (!prop.has_value()) {
                return {};
            }
            lock.unlock();
            auto [result, ok] = postPositionProperty.insert({dataset, prop.value()});
            if (!ok) {
                return {};
            }
            it = result;
        }

        return synapse->getProperty<rush::Vec3f>(it->second);
    }
} // namespace neoneuron