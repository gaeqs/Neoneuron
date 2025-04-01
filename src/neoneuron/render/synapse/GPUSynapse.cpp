//
// Created by gaeqs on 31/03/25.
//

#include "GPUSynapse.h"

namespace neoneuron
{

    GPUSynapse::GPUSynapse(GPUSynapse&& other) noexcept :
        _instanceData(other._instanceData),
        _instance(other._instance),
        _valid(other._valid),
        _prePosition(other._prePosition),
        _postPosition(other._postPosition)
    {
        other._valid = false;
    }

    GPUSynapse::GPUSynapse(const GPUSynapseCreateInfo& info, const mindset::Synapse* synapse) :
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

    void GPUSynapse::refreshGPUData(const GPUSynapseCreateInfo& info, const mindset::Synapse* synapse)
    {
        if (!_valid) {
            return;
        }
        _prePosition = synapse->getProperty<rush::Vec3f>(info.prePositionProperty);
        _postPosition = synapse->getProperty<rush::Vec3f>(info.postPositionProperty);

        GPUSynapseData data;
        data.uid = synapse->getUID();
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