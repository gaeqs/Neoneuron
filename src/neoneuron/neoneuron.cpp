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

#include <cmrc/cmrc.hpp>
#include <imgui_impl_vulkan.h>
#include <neon/logging/Logger.h>
#include <nfd.hpp>

#include <neoneuron/render/NeoneuronRender.h>

#include "application/NeoneuronApplication.h"

CMRC_DECLARE(resources);

int main()
{
    NFD::Init();

    using MeshFeature = VkPhysicalDeviceMeshShaderFeaturesEXT;
    constexpr VkStructureType MESH_FEATURE = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;

    neon::vulkan::VKApplicationCreateInfo info;
    info.name = "Neoneuron";

    info.extraFeatures.emplace_back(MeshFeature{MESH_FEATURE});

    info.deviceFilter = [](const neon::vulkan::VKPhysicalDevice& device) {
        if (!neon::vulkan::VKApplicationCreateInfo::defaultDeviceFilter(device)) {
            return false;
        }

        if (!device.getFeatures().basicFeatures.independentBlend) {
            return false;
        }

        auto feature = device.getFeatures().findFeature<MeshFeature>(MESH_FEATURE);
        if (!feature.has_value()) {
            return false;
        }
        return feature.value()->meshShader > 0;
    };

    info.featuresConfigurator = [](const auto& device, neon::vulkan::VKPhysicalDeviceFeatures& features) {
        neon::vulkan::VKApplicationCreateInfo::defaultFeaturesConfigurer(device, features);
        features.basicFeatures.samplerAnisotropy = true;
        features.basicFeatures.independentBlend = true;
        features.basicFeatures.fillModeNonSolid = true;
        auto* mesh = features.findFeature<MeshFeature>(MESH_FEATURE).value();
        mesh->meshShader = true;
        mesh->taskShader = true;
        features.extensions.emplace_back(VK_EXT_MESH_SHADER_EXTENSION_NAME);
    };

    info.vSync = false;

    neoneuron::NeoneuronApplication app(info);

    //app.getRender().setSkybox(loadSkybox(app.getRender().getRoom().get()));

    bool result = app.getRender().renderLoop();
    app.saveSettings();
    return result ? 0 : 1;
}
