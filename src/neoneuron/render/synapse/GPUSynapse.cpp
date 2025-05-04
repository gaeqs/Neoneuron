

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

#include "GPUSynapse.h"

#include <neon/logging/Logger.h>

namespace neoneuron
{

    GPUSynapse::GPUSynapse(GPUSynapse&& other) noexcept :
        _gid(other._gid),
        _instanceData(other._instanceData),
        _instance(other._instance),
        _valid(other._valid),
        _prePosition(other._prePosition),
        _postPosition(other._postPosition)
    {
        other._valid = false;
    }

    GPUSynapse::GPUSynapse(GPUSynapseCreateInfo& info, GID gid, const mindset::Synapse* synapse) :
        _gid(gid),
        _instanceData(info.instanceData),
        _valid(true)
    {
        auto result = info.instanceData->createInstance();
        if (!result.isOk()) {
            neon::error() << result.getError();
            return;
        }
        _instance = result.getResult();

        refreshGPUData(info, synapse);
    }

    GPUSynapse::~GPUSynapse()
    {
        if (_valid) {
            _instanceData->freeInstance(_instance);
        }
    }

    std::optional<rush::Vec3f> GPUSynapse::getPrePosition() const
    {
        return _prePosition;
    }

    std::optional<rush::Vec3f> GPUSynapse::getPostPosition() const
    {
        return _postPosition;
    }

    void GPUSynapse::refreshGPUData(GPUSynapseCreateInfo& info, const mindset::Synapse* synapse)
    {
        if (!_valid) {
            return;
        }
        _prePosition = info.getPrePositionProperty(_gid.datasetId, synapse);
        _postPosition = info.getPostPositionProperty(_gid.datasetId, synapse);

        GPUSynapseData data;
        data.uid = _gid.internalId;
        data.preNeuronId = synapse->getPreSynapticNeuron();
        data.postNeuronId = synapse->getPostSynapticNeuron();
        data.property = 0.0f;

        if (_prePosition) {
            data.prePosition = rush::Vec4f(*_prePosition, 1.0f);
        } else {
            data.prePosition = rush::Vec4f(0.0f);
        }

        if (_postPosition) {
            data.postPosition = rush::Vec4f(*_postPosition, 1.0f);
        } else {
            data.postPosition = rush::Vec4f(0.0f);
        }

        info.instanceData->uploadData(_instance, 0, data);
    }

} // namespace neoneuron