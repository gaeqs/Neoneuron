//
// Created by gaeqs on 15/01/2025.
//

#include "ActionShuffle.h"

#include <imgui.h>
#include <mnemea/DefaultProperties.h>
#include <mnemea/util/NeuronTransform.h>

namespace neoneuron {
    void ActionShuffle::shuffle(mnemea::UID prop, mnemea::Neuron* neuron) {
        std::uniform_real_distribution dis(-1.0f, 1.0f);
        std::uniform_real_distribution rDis(0.0f, 1.0f);

        auto v = rush::Vec3f(dis(_randomGenerator), dis(_randomGenerator), dis(_randomGenerator));
        v = v.normalized() * std::cbrt(rDis(_randomGenerator)) * _radius;

        mnemea::NeuronTransform transform;
        transform.setPosition(_center + v);

        if (_shuffleRotation) {
            std::uniform_real_distribution rotDis(0.0f, std::numbers::pi_v<float> * 2.0f);
            auto r = rush::Vec3f(rotDis(_randomGenerator), rotDis(_randomGenerator), rotDis(_randomGenerator));
            transform.setRotation(r);
        }

        neuron->setPropertyAsAny(prop, transform);
        _scene->refreshNeuronProperty(neuron->getUID(), mnemea::PROPERTY_TRANSFORM);
    }

    void ActionShuffle::run() {
        auto propId = _scene->getDataset().getProperties().defineProperty(mnemea::PROPERTY_TRANSFORM);
        auto& neurons = _scene->getSelector().getSelectedNeurons();
        if (neurons.empty()) {
            // Shuffle all neurons
            for (auto& neuron: _scene->getDataset().getNeurons()) {
                shuffle(propId, &neuron);
            }
        } else {
            for (auto& uid: neurons) {
                if (auto neuron = _scene->findPrototypeNeuron(uid); neuron.has_value()) {
                    shuffle(propId, neuron.value());
                }
            }
        }
    }

    ActionShuffle::ActionShuffle(AbstractNeuronScene* scene)
        : ModalComponent("Shuffle", true),
          _scene(scene),
          _radius(1000.0f),
          _shuffleRotation(true),
          _randomGenerator(_randomDevice()) {}

    void ActionShuffle::onModalDraw() {
        ImGui::TextWrapped("This tool shuffles the position of all selected neurons. "
            "If no neurons are selected, this tool will shuffle all neurons in the scene.");
        ImGui::Separator();
        ImGui::InputFloat3("Center", _center.toPointer());
        ImGui::InputFloat("Radius", &_radius);
        ImGui::Checkbox("Shuffle rotation", &_shuffleRotation);
    }

    void ActionShuffle::actionButton(ImVec2 recommendedSize) {
        if (ImGui::Button("Run", recommendedSize)) {
            run();
            ImGui::CloseCurrentPopup();
        }
    }
}
