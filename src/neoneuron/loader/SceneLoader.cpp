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

#include "SceneLoader.h"

#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{
    nlohmann::json saveScene(NeoneuronRender* render)
    {
        nlohmann::json json;
        /* nlohmann::json jsonNeurons = nlohmann::json::array();

        auto& props = render->getNeoneuronApplication()->getPropertyStorage();

        for (auto& neuron: render->getNeuronScene()->getDataset().getNeurons()) {
            nlohmann::json jsonNeuron = nlohmann::json::object();
            jsonNeuron["id"] = neuron.getUID();

            nlohmann::json jsonGlobalProperties = nlohmann::json::object();
            auto& propNames = neuron->getPropertiesNames();
            for (auto& [uid, value]: neuron->getGlobalProperties()) {
                auto propName = propNames.find(uid);
                if (propName == propNames.end()) continue;
                auto optional = props.get(propName->second);
                if (!optional.has_value()) continue;
                jsonGlobalProperties[propName->second] = optional.value()->getToJson()(value);;
            }
            jsonNeuron["properties"] = jsonGlobalProperties;

            // Sections
            nlohmann::json jsonSegments = nlohmann::json::array();
            for (auto& segment: neuron->getSegments()) {
                nlohmann::json jsonSegment = nlohmann::json::object();
                jsonSegment["id"] = segment.getId();
                nlohmann::json jsonProperties = nlohmann::json::object();
                for (auto& [uid, value]: segment.getProperties()) {
                    auto propName = propNames.find(uid);
                    if (propName == propNames.end()) continue;

                    auto optional = props.get(propName->second);
                    if (!optional.has_value()) {
                        std::cout << propName->second << std::endl;
                        continue;
                    }
                    jsonProperties[propName->second] = optional.value()->getToJson()(value);;
                }
                jsonSegment["properties"] = jsonProperties;
                jsonSegments.push_back(jsonSegment);
            }

            jsonNeuron["segments"] = jsonSegments;
            jsonNeurons.push_back(jsonNeuron);
        }
        json["neurons"] = jsonNeurons;
*/
        return json;
    }
} // namespace neoneuron
