// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
