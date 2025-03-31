//
// Created by gaeqs on 23/12/2024.
//

#ifndef NEONEURON_DEFAULTPROPERTIES_H
#define NEONEURON_DEFAULTPROPERTIES_H

#include <mindset/Neuron.h>
#include <nlohmann/json_fwd.hpp>
#include <neoneuron/render/AbstractNeuronRepresentation.h>

namespace neoneuron::property
{
    bool transformEditor(std::any* property, const mindset::Neuron* neuron);

    std::any transformGenerator(const mindset::Neuron* neuron);

    std::any transformFromJson(const nlohmann::json& json);

    nlohmann::json transformToJson(const std::any& any);

    bool nameEditor(std::any* property, const mindset::Neuron* neuron);

    std::any nameGenerator(const mindset::Neuron* neuron);

    std::any nameFromJson(const nlohmann::json& json);

    nlohmann::json nameToJson(const std::any& any);

    bool lodEditor(std::any* property, const mindset::Neuron* neuron);

    std::any lodGenerator(const mindset::Neuron* neuron);

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

} // namespace neoneuron::property

#endif //NEONEURON_DEFAULTPROPERTIES_H
