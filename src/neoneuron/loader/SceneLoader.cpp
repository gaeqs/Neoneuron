//
// Created by gaeqs on 14/02/25.
//

#include "SceneLoader.h"

#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron {
    nlohmann::json saveScene(NeoneuronRender* render) {
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
}
