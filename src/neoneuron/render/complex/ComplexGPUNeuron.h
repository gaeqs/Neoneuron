//
// Created by gaeqs on 10/10/24.
//

#ifndef COMPLEXGPUNEURON_H
#define COMPLEXGPUNEURON_H


#include <neon/Neon.h>
#include <neoneuron/structure/complex/ComplexNeuron.h>

namespace neoneuron {
    struct ComplexGPUNeuronGlobalData {
        uint32_t neuronId;
        uint32_t dummy[3];
        rush::Mat4f model;
    };

    struct ComplexGPUNeuronSegment {
        /**
        * The neuron's global data's position in the storage buffer.
        */
        uint32_t neuronIndex;

        /**
        * The UID of the segment.
        */
        uint32_t segmentId;

        /**
        * The type of the segment.
        * 0-7 -> Type
        * 8 - 10 -> Amount of children.
        */
        uint32_t metadata;

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
        * The parent's position in the storage buffer.
        */
        uint32_t parent;

        /**
        * The amount of connections.
        */
        uint32_t amount;

        /**
        * The amount of vertices the split will rotate.
        */
        uint32_t rotationIndex;

        /**
         * The connections' positions in the storage buffer.
         */
        uint32_t connections[8];
    };

    struct ComplexGPUNeuronSoma {
        /*
        * The index of the representing section in the storage buffer.
        */
        uint32_t sectionIndex;

        /**
        * The amount of connections.
        */
        uint32_t amount;

        /**
         * The connections' positions in the storage buffer.
         */
        uint32_t connections[8];
    };

    class ComplexGPUNeuron {
        std::weak_ptr<neon::InstanceData> _globalInstanceData;
        std::weak_ptr<neon::Model> _segmentModel;
        std::weak_ptr<neon::Model> _jointModel;
        std::weak_ptr<neon::Model> _somaModel;
        size_t _segmentInstanceDataIndex;
        size_t _jointInstanceDataIndex;
        size_t _somaInstanceDataIndex;
        neon::InstanceData::Instance _globalInstance;
        std::vector<neon::InstanceData::Instance> _segmentInstances;
        std::unordered_map<UID, neon::InstanceData::Instance> _segmentInstancesByUID;
        std::vector<neon::InstanceData::Instance> _jointInstances;
        std::unordered_map<UID, neon::InstanceData::Instance> _jointInstancesByUID;
        std::vector<neon::InstanceData::Instance> _somaInstances;
        std::unordered_map<UID, neon::InstanceData::Instance> _somaInstancesByUID;
        const ComplexNeuron* _neuron;
        bool _valid;


        void generateSegmentInstances();

        void generateJointInstances();

        void generateSomaInstances();

        void refreshGlobalData() const;

        void refreshSegments() const;

        void refreshJoints() const;

        void refreshSomas() const;

    public:
        ComplexGPUNeuron(ComplexGPUNeuron&& other) noexcept;

        ComplexGPUNeuron(const ComplexGPUNeuron& other) = delete;

        ComplexGPUNeuron(std::weak_ptr<neon::InstanceData> globalInstanceData,
                         std::weak_ptr<neon::Model> neuronModel,
                         std::weak_ptr<neon::Model> jointModel,
                         std::weak_ptr<neon::Model> somaModel,
                         size_t segmentInstanceDataIndex,
                         size_t jointInstanceDataIndex,
                         size_t somaInstanceDataIndex,
                         const ComplexNeuron* neuron);

        ~ComplexGPUNeuron();

        void refreshGPUData() const;

        void refreshProperty(const std::string& propertyName);

        std::optional<neon::InstanceData::Instance> findSegment(UID uid) const;

        std::optional<neon::InstanceData::Instance> findJoint(UID uid) const;

        ComplexGPUNeuron& operator=(ComplexGPUNeuron&& other) noexcept;
    };
}

#endif //COMPLEXGPUNEURON_H
