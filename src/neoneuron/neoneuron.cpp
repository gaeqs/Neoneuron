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

/*std::string text = R"(<?xml version="1.0" encoding="UTF-8"?>

<scene version="0.1">
  <morphology>
    <columns>
      <column id="0">
        <minicolumn id="0">
          <neuron gid="0" layer="2">
          </neuron>
          <neuron gid="1" layer="3" type="INTERNEURON">
            <transform>
              -1.0, 0, 0.0, 100.0,
              0.0, -1, 0.0, 0.0,
              0.0, 0.0, 1.0, 150.0,
              0.0, 0.0, 0.0, 1.0
            </transform>
          </neuron>
        </minicolumn>
        <minicolumn id="1">
          <neuron gid="2" type="PYRAMIDAL">
            <transform>
              1.0, 0.0, 0.0, 200.0,
              0.0, 1.0, 0.0, 0.0,
              0.0, 0.0, 1.0, 50.0,
              0.0, 0.0, 0.0, 1
            </transform>
          </neuron>
        </minicolumn>
      </column>
      <column id="1">
        <minicolumn id="0">
          <neuron gid="3" type="PYRAMIDAL">
            <transform>
              -1.0, 0, 0.0, 300.0,
              0.0, -1.0, 0.0, 0.0,
              0.0, 0.0, 1.0, 200.0,
              0.0, 0.0, 0.0, 1
            </transform>
          </neuron>
        </minicolumn>
      </column>
    </columns>
    <neuronmorphologies>
      <neuronmorphology neurons="0,1" swc="GA02326c1n2-2.CNG.swc" />
      <neuronmorphology neurons="2,3" swc="GA02327c1.CNG.swc" />
    </neuronmorphologies>
  </morphology>
</scene>
)";*/

int main() {
    //neoneuron::XMLLoader loader(nullptr, "", text.data(), text.size());
    //loader.build(0);

    //return 0;

    NFD::Init();

    using MeshFeature = VkPhysicalDeviceMeshShaderFeaturesEXT;
    constexpr VkStructureType MESH_FEATURE = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;

    auto logger = neon::Logger(true, true, true);
    logger.debug("Hello world!");

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
    bool result = app.getRender().renderLoop();
    app.saveSettings();
    return result ? 0 : 1;
}
