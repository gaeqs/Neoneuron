//
// Created by gaeqs on 10/10/24.
//

#ifndef COMPLEXGPUNEURON_H
#define COMPLEXGPUNEURON_H


#include <neon/Neon.h>
#include <neoneuron/structure/complex/ComplexNeuron.h>

namespace neoneuron {
    struct ComplexGPUNeuronSegment {
        /**
        * The UID of the neuron this segment is inside.
        */
        uint32_t neuronId;

        /**
        * The UID of the segment.
        */
        uint32_t segmentId;

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

    struct ComplexGPUNeuronJoint {
        /**
        * The amount of connections.
        */
        uint32_t amount;

        /**
        * The parent's position in the storage buffer.
*/
        uint32_t parent;

        /**
         * The connections' positions in the storage buffer.
         */
        uint32_t connections[8];
    };

    class ComplexGPUNeuron {
        std::weak_ptr<neon::Model> _model;
        size_t _instanceDataIndex;
        std::vector<neon::InstanceData::Instance> _instances;
        std::unordered_map<UID, neon::InstanceData::Instance> _instancesByUID;
        const ComplexNeuron* _neuron;
        bool _valid;

    public:
        ComplexGPUNeuron(ComplexGPUNeuron&& other) noexcept;

        ComplexGPUNeuron(const ComplexGPUNeuron& other) = delete;

        ComplexGPUNeuron(std::weak_ptr<neon::Model> model,
                         size_t instanceDataIndex,
                         const ComplexNeuron* neuron);

        ~ComplexGPUNeuron();

        void refreshGPUData() const;

        std::optional<neon::InstanceData::Instance> findSegment(UID uid) const;

        ComplexGPUNeuron& operator=(ComplexGPUNeuron&& other) noexcept;
    };
}

#endif //COMPLEXGPUNEURON_H
