//
// Created by gaeqs on 8/10/24.
//


#include <imgui_impl_vulkan.h>
#include <nfd.hpp>
#include <cmrc/cmrc.hpp>
#include <neon/logging/Logger.h>

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/loader/SWCLoader.h>

#include "application/NeoneuronApplication.h"
#include "loader/XMLLoader.h"


CMRC_DECLARE(resources);

std::shared_ptr<neon::Texture> loadSkybox(neon::Room* room) {
    neon::CMRCFileSystem fileSystem(cmrc::resources::get_filesystem());
    neon::AssetLoaderContext context(room->getApplication(), nullptr, &fileSystem);
    return loadAssetFromFile<neon::Texture>("texture/skybox/skybox.json", context);
}

int main() {
    NFD::Init();

    using MeshFeature = VkPhysicalDeviceMeshShaderFeaturesEXT;
    constexpr VkStructureType MESH_FEATURE = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;

    neon::vulkan::VKApplicationCreateInfo info;
    info.name = "Neoneuron";

    info.extraFeatures.emplace_back(MeshFeature{MESH_FEATURE});

    info.deviceFilter = [](const neon::vulkan::VKPhysicalDevice& device) {
        if (!neon::vulkan::VKApplicationCreateInfo::defaultDeviceFilter(device)) return false;

        if (!device.getFeatures().basicFeatures.independentBlend) return false;

        auto feature = device.getFeatures().findFeature<MeshFeature>(MESH_FEATURE);
        if (!feature.has_value()) return false;
        return feature.value()->meshShader > 0;
    };

    info.featuresConfigurator = [](const auto& device, neon::vulkan::VKPhysicalDeviceFeatures& features) {
        neon::vulkan::VKApplicationCreateInfo::defaultFeaturesConfigurer(device, features);
        features.basicFeatures.independentBlend = true;
        features.basicFeatures.fillModeNonSolid = true;
        auto* mesh = features.findFeature<MeshFeature>(MESH_FEATURE).value();
        mesh->meshShader = true;
        mesh->taskShader = true;
        features.extensions.emplace_back(VK_EXT_MESH_SHADER_EXTENSION_NAME);
    };

    info.vSync = false;

    neoneuron::NeoneuronApplication app(info);

    app.getRender().setSkybox(loadSkybox(app.getRender().getRoom().get()));

    bool result = app.getRender().renderLoop();
    app.saveSettings();
    return result ? 0 : 1;
}
