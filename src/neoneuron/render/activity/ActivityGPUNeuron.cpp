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

#include "ActivityGPUNeuron.h"

#include <neon/logging/Logger.h>

namespace neoneuron
{

    ActivityGPUNeuron::ActivityGPUNeuron(ActivityGPUNeuron&& other) noexcept :
        _gid(other._gid),
        _position(other._position),
        _instanceData(other._instanceData),
        _instance(other._instance),
        _valid(other._valid)
    {
        other._valid = false;
    }

    ActivityGPUNeuron::ActivityGPUNeuron(neon::InstanceData* instanceData, GID gid, rush::Vec3f position, float value) :
        _gid(gid),
        _position(position),
        _instanceData(instanceData),
        _valid(true)
    {
        auto result = instanceData->createInstance();
        if (!result.isOk()) {
            neon::error() << result.getError();
            _valid = false;
            return;
        }
        _instance = result.getResult();

        refreshGPUData();
        updateActivityValue(value);
    }

    ActivityGPUNeuron::~ActivityGPUNeuron()
    {
        if (_valid) {
            _instanceData->freeInstance(_instance);
        }
    }

    rush::Vec3f ActivityGPUNeuron::getPosition() const
    {
        return _position;
    }

    void ActivityGPUNeuron::refreshGPUData() const
    {
        if (!_valid) {
            return;
        }
        ActivityGPUNeuronData data;
        data.datasetId = _gid.datasetId;
        data.neuronId = _gid.internalId;
        data.postPosition = rush::Vec4f(_position, 1.0f);
        _instanceData->uploadData(_instance, 0, data);
    }

    void ActivityGPUNeuron::updateActivityValue(float value) const
    {
        _instanceData->uploadData(_instance, 1, value);
    }
} // namespace neoneuron