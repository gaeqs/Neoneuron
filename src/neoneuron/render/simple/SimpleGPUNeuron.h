//
// Created by gaeqs on 10/10/24.
//

#ifndef GPUNEURON_H
#define GPUNEURON_H


#include <neon/Neon.h>
#include <neoneuron/structure/simple/SimpleNeuron.h>

namespace neoneuron {
    struct SimpleGPUNeuronSegment {
        uint32_t neuronId;

        uint32_t sectionId;

        /**
        * The type of the segment.
        */
        uint32_t type;

        /**
        * The position of the parent in the storage buffer.
        */
        uint32_t parent;

        /**
        * The end position of the segment (x, y, z) and the radius (w).
        * This merge is done to save a float position (vec3 has the same padding as vec4 in std430)
        */
        rush::Vec4f endAndRadius;
    };

    class SimpleGPUNeuron {
        std::weak_ptr<neon::Model> _model;
        size_t _instanceDataIndex;
        std::vector<neon::InstanceData::Instance> _instances;
        std::unordered_map<UID, neon::InstanceData::Instance> _instancesByUID;
        const SimpleNeuron* _neuron;
        bool _valid;

    public:
        SimpleGPUNeuron(SimpleGPUNeuron&& other) noexcept;

        SimpleGPUNeuron(const SimpleGPUNeuron& other) = delete;

        SimpleGPUNeuron(std::weak_ptr<neon::Model> model,
                  size_t instanceDataIndex,
                  const SimpleNeuron* neuron);

        ~SimpleGPUNeuron();

        void refreshGPUData() const;

        std::optional<neon::InstanceData::Instance> findSegment(UID uid) const;

        SimpleGPUNeuron& operator=(SimpleGPUNeuron&& other) noexcept;
    };
}

#endif //GPUNEURON_H
