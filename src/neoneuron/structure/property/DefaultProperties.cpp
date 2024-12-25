//
// Created by gaeqs on 23/12/2024.
//

#include "DefaultProperties.h"

#include <imgui.h>
#include <neoneuron/structure/NeuronTransform.h>

bool neoneuron::property::transformEditor(std::any* property,
                                          const PrototypeNeuron* neuron,
                                          const AbstractNeuronScene* scene) {
    auto* t = std::any_cast<NeuronTransform>(property);
    if (t == nullptr) return false;

    auto p = t->getPosition();
    auto r = rush::degrees(t->getRotation().euler());
    auto s = t->getScale();
    bool updated = false;

    ImGui::PushItemWidth(-1.0f);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Position:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##position", &p.x(), 0.1f)) {
        t->setPosition(p);
        updated = true;
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Rotation:");
    ImGui::SameLine();

    if (ImGui::DragFloat3("##rotation", &r.x(), 0.1f)) {
        auto rotation = rush::Quatf::euler(rush::radians(r));
        t->setRotation(rotation);
        updated = true;
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Scale:");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##scale", s.toPointer(), 0.02f)) {
        t->setScale(s);
        updated = true;
    }

    ImGui::PopItemWidth();

    return updated;
}

std::any neoneuron::property::transformGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene) {
    return NeuronTransform();
}

bool neoneuron::property::nameEditor(std::any* property, const PrototypeNeuron* neuron,
                                     const AbstractNeuronScene* scene) {
    constexpr size_t BUF_SIZE = 64;
    auto* s = std::any_cast<std::string>(property);
    if (s == nullptr) return false;

    char buf[BUF_SIZE];
    size_t size = std::min(s->size(), BUF_SIZE - 1);
    std::memcpy(buf, s->data(), size);
    buf[size] = '\0';

    ImGui::PushItemWidth(-1.0f);

    ImGui::Text("Name:");
    ImGui::SameLine();

    if (ImGui::InputText("##name", buf, BUF_SIZE - 1)) {
        *s = std::string(buf);
        ImGui::PopItemWidth();
        return true;
    }

    ImGui::PopItemWidth();
    return false;
}

std::any neoneuron::property::nameGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene) {
    return std::string();
}
