//
// Created by gaeqs on 8/10/24.
//

#include <cmrc/cmrc.hpp>
#include <neon/logging/Logger.h>

#include <neoneuron/render/NeoneuronRender.h>

#include "loader/SWCLoader.h"

CMRC_DECLARE(resources);

int main() {
    using MeshFeature = VkPhysicalDeviceMeshShaderFeaturesEXT;
    constexpr VkStructureType MESH_FEATURE = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;

    auto logger = neon::Logger(true, true, true);
    logger.debug("Hello world!");

    neon::vulkan::VKApplicationCreateInfo info;
    info.name = "Neoneuron";

    info.extraFeatures.emplace_back(MeshFeature{MESH_FEATURE});

    info.deviceFilter = [](const neon::vulkan::VKPhysicalDevice& device) {
        if (!neon::vulkan::VKApplicationCreateInfo::defaultDeviceFilter(device)) return false;
        auto feature = device.getFeatures().findFeature<MeshFeature>(MESH_FEATURE);
        if (!feature.has_value()) return false;
        return feature.value()->meshShader > 0;
    };

    info.featuresConfigurator = [](const auto& device, neon::vulkan::VKPhysicalDeviceFeatures& features) {
        neon::vulkan::VKApplicationCreateInfo::defaultFeaturesConfigurer(device, features);
        auto* mesh = features.findFeature<MeshFeature>(MESH_FEATURE).value();
        mesh->meshShader = true;
        mesh->taskShader = true;
        features.extensions.emplace_back(VK_EXT_MESH_SHADER_EXTENSION_NAME);
    };

    neoneuron::NeoneuronRender render(info);

    auto file = cmrc::resources::get_filesystem().open("data.swc");
    std::string fileData(file.begin(), file.end());
    std::stringstream ss(fileData);
    neoneuron::SWCLoader loader(ss);

    auto result = loader.build(1);

    if (!result.isOk()) {
        logger.error(result.getError());
        return 0;
    }

    neoneuron::Neuron neuron = std::move(result.getResult());
    render.getNeuronScene().addNeuron(std::move(neuron));

    return render.renderLoop() ? 0 : 1;
}
