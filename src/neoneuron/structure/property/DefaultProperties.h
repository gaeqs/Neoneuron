//
// Created by gaeqs on 23/12/2024.
//

#ifndef DEFAULTPROPERTIES_H
#define DEFAULTPROPERTIES_H
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>
#include <nlohmann/json_fwd.hpp>


namespace neoneuron::property {
    bool transformEditor(std::any* property, const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);

    std::any transformGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);

    std::any transformFromJson(const nlohmann::json& json);

    nlohmann::json transformToJson(const std::any& any);

    bool nameEditor(std::any* property, const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);

    std::any nameGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);

    std::any nameFromJson(const nlohmann::json& json);

    nlohmann::json nameToJson(const std::any& any);

    bool lodEditor(std::any* property, const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);

    std::any lodGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);

    std::any int32FromJson(const nlohmann::json& json);

    nlohmann::json int32ToJson(const std::any& any);

    std::any int64FromJson(const nlohmann::json& json);

    nlohmann::json int64ToJson(const std::any& any);

    std::any uint32FromJson(const nlohmann::json& json);

    nlohmann::json uint32ToJson(const std::any& any);

    std::any floatFromJson(const nlohmann::json& json);

    nlohmann::json floatToJson(const std::any& any);

    std::any vec3fFromJson(const nlohmann::json& json);

    nlohmann::json vec3fToJson(const std::any& any);

}


#endif //DEFAULTPROPERTIES_H
