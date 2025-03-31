//
// Created by gaeqs on 15/01/2025.
//

#include "ActionShuffle.h"

#include <imgui.h>
#include <mindset/DefaultProperties.h>
#include <mindset/util/NeuronTransform.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>

namespace neoneuron
{
    void ActionShuffle::shuffle(mindset::UID prop, mindset::Neuron* neuron)
    {
        std::uniform_real_distribution dis(-1.0f, 1.0f);
        std::uniform_real_distribution rDis(0.0f, 1.0f);

        auto v = rush::Vec3f(dis(_randomGenerator), dis(_randomGenerator), dis(_randomGenerator));
        v = v.normalized() * std::cbrt(rDis(_randomGenerator)) * _radius;

        mindset::NeuronTransform transform;
        transform.setPosition(_center + v);

        if (_shuffleRotation) {
            std::uniform_real_distribution rotDis(0.0f, std::numbers::pi_v<float> * 2.0f);
            auto r = rush::Vec3f(rotDis(_randomGenerator), rotDis(_randomGenerator), rotDis(_randomGenerator));
            transform.setRotation(r);
        }

        neuron->setProperty(prop, transform);
        _scene->refreshNeuronProperty(neuron->getUID(), mindset::PROPERTY_TRANSFORM);
    }

    void ActionShuffle::run()
    {
        auto& dataset = _scene->getRender()->getNeoneuronApplication()->getDataset();
        auto propId = dataset.getProperties().defineProperty(mindset::PROPERTY_TRANSFORM);

        auto& neurons = _scene->getSelector().getSelectedNeurons();
        if (neurons.empty()) {
            // Shuffle all neurons
            for (auto neuron : dataset.getNeurons()) {
                shuffle(propId, neuron.getRaw());
            }
        } else {
            for (auto& uid : neurons) {
                if (auto neuron = dataset.getNeuron(uid); neuron.has_value()) {
                    shuffle(propId, neuron.value());
                }
            }
        }
    }

    ActionShuffle::ActionShuffle(AbstractNeuronRepresentation* scene) :
        ModalComponent("Shuffle", true),
        _scene(scene),
        _radius(1000.0f),
        _shuffleRotation(true),
        _randomGenerator(_randomDevice())
    {
    }

    void ActionShuffle::onModalDraw()
    {
        ImGui::TextWrapped("This tool shuffles the position of all selected neurons. "
                           "If no neurons are selected, this tool will shuffle all neurons in the scene.");
        ImGui::Separator();
        ImGui::InputFloat3("Center", _center.toPointer());
        ImGui::InputFloat("Radius", &_radius);
        ImGui::Checkbox("Shuffle rotation", &_shuffleRotation);
    }

    void ActionShuffle::actionButton(ImVec2 recommendedSize)
    {
        if (ImGui::Button("Run", recommendedSize)) {
            run();
            ImGui::CloseCurrentPopup();
        }
    }
} // namespace neoneuron
