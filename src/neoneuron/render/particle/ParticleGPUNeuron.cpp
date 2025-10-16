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

#include "ParticleGPUNeuron.h"

#include <neon/logging/Logger.h>

namespace neoneuron
{

    ParticleGPUNeuron::ParticleGPUNeuron(ParticleGPUNeuron&& other) noexcept :
        _gid(other._gid),
        _position(other._position),
        _colorAndScaleIndex(other._colorAndScaleIndex),
        _instanceData(other._instanceData),
        _instance(other._instance),
        _valid(other._valid)
    {
        other._valid = false;
    }

    ParticleGPUNeuron::ParticleGPUNeuron(neon::InstanceData* instanceData, GID gid, rush::Vec3f position) :
        _gid(gid),
        _position(position),
        _colorAndScaleIndex(0),
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
    }

    ParticleGPUNeuron::~ParticleGPUNeuron()
    {
        if (_valid) {
            _instanceData->freeInstance(_instance);
        }
    }

    GID ParticleGPUNeuron::getGID() const
    {
        return _gid;
    }

    rush::Vec3f ParticleGPUNeuron::getPosition() const
    {
        return _position;
    }

    void ParticleGPUNeuron::setColorAndScaleIndex(uint32_t colorAndScaleIndex)
    {
        _colorAndScaleIndex = colorAndScaleIndex;
        refreshGPUData();
    }

    void ParticleGPUNeuron::setPosition(rush::Vec3f position)
    {
        _position = position;
        refreshGPUData();
    }

    void ParticleGPUNeuron::refreshGPUData() const
    {
        if (!_valid) {
            return;
        }
        ActivityGPUNeuronData data;
        data.datasetId = _gid.datasetId;
        data.neuronId = _gid.internalId;
        data.colorAndScaleIndex = _colorAndScaleIndex;
        data.postPosition = rush::Vec4f(_position, 1.0f);
        _instanceData->uploadData(_instance, 0, data);
    }
} // namespace neoneuron