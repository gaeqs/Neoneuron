//
// Created by gaeqs on 23/12/2024.
//

#include "DefaultProperties.h"

#include <imgui.h>
#include <neoneuron/structure/NeuronTransform.h>
#include <nlohmann/json.hpp>

namespace neoneuron::property {
    bool transformEditor(std::any* property, const PrototypeNeuron* neuron, const AbstractNeuronScene* scene) {
        auto* t = std::any_cast<NeuronTransform>(property);
        if (t == nullptr) return false;

        auto p = t->getPosition();
        auto r = rush::degrees(t->getRotation());
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
            t->setRotation(rush::radians(r));
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

    std::any transformGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene) {
        return NeuronTransform();
    }

    std::any transformFromJson(const nlohmann::json& json) {
        NeuronTransform t;
        if (json.contains("rotation")) {
            if (auto& rot = json["rotation"]; rot.is_object()) {
                rush::Vec3f result;
                if (rot.contains("pitch")) {
                    if (auto& x = rot["pitch"]; x.is_number()) {
                        result[0] = x.get<float>();
                    }
                }
                if (rot.contains("yaw")) {
                    if (auto& y = rot["yaw"]; y.is_number()) {
                        result[1] = y.get<float>();
                    }
                }
                if (rot.contains("roll")) {
                    if (auto& z = rot["roll"]; z.is_number()) {
                        result[2] = z.get<float>();
                    }
                }

                t.setRotation(result);
            }
        }

        if (json.contains("scale")) {
            if (auto& scale = json["scale"]; scale.is_object()) {
                rush::Vec3f result;
                if (scale.contains("x")) {
                    if (auto& x = scale["x"]; x.is_number()) {
                        result[0] = x.get<float>();
                    }
                }
                if (scale.contains("y")) {
                    if (auto& y = scale["y"]; y.is_number()) {
                        result[1] = y.get<float>();
                    }
                }
                if (scale.contains("z")) {
                    if (auto& z = scale["z"]; z.is_number()) {
                        result[2] = z.get<float>();
                    }
                }

                t.setScale(result);
            }
        }

        if (json.contains("position")) {
            if (auto& pos = json["position"]; pos.is_object()) {
                rush::Vec3f result;
                if (pos.contains("x")) {
                    if (auto& x = pos["x"]; x.is_number()) {
                        result[0] = x.get<float>();
                    }
                }
                if (pos.contains("y")) {
                    if (auto& y = pos["y"]; y.is_number()) {
                        result[1] = y.get<float>();
                    }
                }
                if (pos.contains("z")) {
                    if (auto& z = pos["z"]; z.is_number()) {
                        result[2] = z.get<float>();
                    }
                }

                t.setPosition(result);
            }
        }
        return t;
    }

    nlohmann::json transformToJson(const std::any& any) {
        const auto* t = std::any_cast<NeuronTransform>(&any);
        if (t == nullptr) return nlohmann::json::object();
        nlohmann::json json = nlohmann::json::object();
        json["rotation"]["pitch"] = t->getRotation().x();
        json["rotation"]["yaw"] = t->getRotation().y();
        json["rotation"]["roll"] = t->getRotation().z();
        json["scale"]["x"] = t->getScale().x();
        json["scale"]["y"] = t->getScale().y();
        json["scale"]["z"] = t->getScale().z();
        json["position"]["x"] = t->getPosition().x();
        json["position"]["y"] = t->getPosition().y();
        json["position"]["z"] = t->getPosition().z();
        return json;
    }

    bool nameEditor(std::any* property, const PrototypeNeuron* neuron, const AbstractNeuronScene* scene) {
        constexpr size_t BUF_SIZE = 64;
        auto* s = std::any_cast<std::string>(property);
        if (s == nullptr) return false;

        char buf[BUF_SIZE];
        size_t size = std::min(s->size(), BUF_SIZE - 1);
        memcpy(buf, s->data(), size);
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

    std::any nameGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene) {
        return std::string();
    }

    std::any nameFromJson(const nlohmann::json& json) {
        return json.get<std::string>();
    }

    nlohmann::json nameToJson(const std::any& any) {
        auto* s = std::any_cast<std::string>(&any);
        if (s == nullptr) return "";
        return *s;
    }

    std::any int32FromJson(const nlohmann::json& json) {
        return json.get<int32_t>();
    }

    nlohmann::json int32ToJson(const std::any& any) {
        auto* i = std::any_cast<int32_t>(&any);
        if (i == nullptr) return 0;
        return *i;
    }

    std::any int64FromJson(const nlohmann::json& json) {
        return json.get<int64_t>();
    }

    nlohmann::json int64ToJson(const std::any& any) {
        auto* i = std::any_cast<int64_t>(&any);
        if (i == nullptr) return 0;
        return *i;
    }

    std::any uint32FromJson(const nlohmann::json& json) {
        return json.get<uint32_t>();
    }

    nlohmann::json uint32ToJson(const std::any& any) {
        auto* i = std::any_cast<uint32_t>(&any);
        if (i == nullptr) return 0;
        return *i;
    }

    std::any floatFromJson(const nlohmann::json& json) {
        return json.get<float>();
    }

    nlohmann::json floatToJson(const std::any& any) {
        auto* i = std::any_cast<float>(&any);
        if (i == nullptr) return 0.0f;
        return *i;
    }

    std::any vec3fFromJson(const nlohmann::json& json) {
        rush::Vec3f result;
        if (json.contains("x")) {
            if (auto& x = json["x"]; x.is_number()) {
                result[0] = x.get<float>();
            }
        }
        if (json.contains("y")) {
            if (auto& y = json["y"]; y.is_number()) {
                result[1] = y.get<float>();
            }
        }
        if (json.contains("z")) {
            if (auto& z = json["z"]; z.is_number()) {
                result[2] = z.get<float>();
            }
        }
        return result;
    }

    nlohmann::json vec3fToJson(const std::any& any) {
        const auto* v = std::any_cast<rush::Vec3f>(&any);
        if (v == nullptr) return nlohmann::json::object();
        nlohmann::json json = nlohmann::json::object();
        json["x"] = v->x();
        json["y"] = v->y();
        json["z"] = v->z();
        return json;
    }
}
