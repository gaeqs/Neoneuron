//
// Created by gaeqs on 9/10/24.
//

#ifndef COMPLEXNEURONSCENE_H
#define COMPLEXNEURONSCENE_H
#include <memory>

#include <neon/Neon.h>
#include <neoneuron/structure/complex/ComplexNeuron.h>
#include <neoneuron/render/complex/ComplexGPUNeuron.h>
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/render/complex/ComplexNeuronSelector.h>


namespace neoneuron {
    class NeoneuronRender;

    class ComplexNeuronScene : public AbstractNeuronScene {
    public:
        static constexpr size_t UNIFORM_SET = 2;
        static constexpr size_t GLOBAL_DATA_BINDING = 0;
        static constexpr size_t SEGMENT_BINDING = 1;
        static constexpr size_t JOINT_BINDING = 2;
        static constexpr size_t SOMA_BINDING = 3;
        static constexpr size_t SELECTION_BINDING = 4;
        static constexpr size_t SOMA_GPU_DATA_BINDING = 5;

        static constexpr size_t SEGMENT_INSTANCES = 10000000;
        static constexpr size_t JOINT_INSTANCES = 10000000;
        static constexpr size_t SOMA_INSTANCES = 100000;
        static constexpr size_t STORAGE_PER_SOMA = 64 * 64;

    private:
        NeoneuronRender* _render;
        ComplexNeuronSelector _selector;

        std::shared_ptr<neon::ShaderUniformDescriptor> _uboDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _ubo;
        std::shared_ptr<neon::StorageBufferInstanceData> _globalInstanceData;

        std::shared_ptr<neon::ShaderProgram> _neuronShader;
        std::shared_ptr<neon::ShaderProgram> _jointShader;
        std::shared_ptr<neon::ShaderProgram> _somaShader;
        std::shared_ptr<neon::Material> _neuronMaterial;
        std::shared_ptr<neon::Material> _jointMaterial;
        std::shared_ptr<neon::Material> _somaMaterial;
        std::shared_ptr<neon::Model> _neuronModel;
        std::shared_ptr<neon::Model> _jointModel;
        std::shared_ptr<neon::Model> _somaModel;

        std::vector<std::shared_ptr<PrototypeNeuron>> _prototypes;
        std::vector<ComplexNeuron> _neurons;
        std::vector<ComplexGPUNeuron> _gpuNeurons;
        rush::AABB<3, float> _sceneBoundingBox;

        bool _wireframe;

        void loadUniformBuffers();

        void loadNeuronShader();

        void loadJointShader();

        void loadSomaShader();

        void loadNeuronMaterial();

        void loadJointMaterial();

        void loadSomaMaterial();

        void loadNeuronModel();

        void loadJointModel();

        void loadSomaModel();

        void combineBoundingBoxes(const rush::AABB<3, float>& aabb);

        void recalculateBoundingBox();

        void reassignMaterials() const;

    public:
        ComplexNeuronScene(const ComplexNeuronScene&) = delete;

        explicit ComplexNeuronScene(NeoneuronRender* render);

        ~ComplexNeuronScene() override;

        [[nodiscard]] NeoneuronRender* getRender() override;

        [[nodiscard]] const NeoneuronRender* getRender() const override;

        [[nodiscard]] const std::vector<ComplexNeuron>& getNeurons() const;

        [[nodiscard]] AbstractSelector& getSelector() override;

        [[nodiscard]] const AbstractSelector& getSelector() const override;

        [[nodiscard]] size_t getNeuronsAmount() override;

        [[nodiscard]] const std::vector<std::shared_ptr<PrototypeNeuron>>& getPrototypeNeurons() const override;

        [[nodiscard]] std::optional<PrototypeNeuron*> findPrototypeNeuron(UID uid) override;

        [[nodiscard]] std::optional<const PrototypeNeuron*> findPrototypeNeuron(UID uid) const override;

        [[nodiscard]] std::optional<ComplexNeuron*> findNeuron(UID uid);

        [[nodiscard]] std::optional<const ComplexNeuron*> findNeuron(UID uid) const;

        [[nodiscard]] std::optional<ComplexGPUNeuron*> findGPUNeuron(UID uid);

        [[nodiscard]] std::optional<const ComplexGPUNeuron*> findGPUNeuron(UID uid) const;

        bool addNeuron(const PrototypeNeuron& neuron) override;

        bool addNeuron(PrototypeNeuron&& neuron) override;

        bool removeNeuron(UID neuronId) override;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;

        void refreshNeuronProperty(UID neuronId, const std::string& propertyName) override;

        [[nodiscard]] bool isWireframeMode() const;

        void setWireframeMode(bool wireframe);

        void reloadShader();
    };
}

#endif //COMPLEXNEURONSCENE_H
