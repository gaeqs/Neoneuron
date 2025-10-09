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
#include <neon/logging/Logger.h>

#include <cxxopts.hpp>
#include <utility>

#include <neoneuron/render/NeoneuronRender.h>

#include "application/NeoneuronApplication.h"

CMRC_DECLARE(resources);

void parseLoaderArguments(const cxxopts::ParseResult& result, neoneuron::NeoneuronApplication& app);

int main(int argc, const char* argv[])
{
    cxxopts::Options options("neoneuron", "A 3D viewer for neurons.");

    options.add_options("General")("h,help", "Print help");

    options.add_options("Graphics")("f,fullscreen", "Start in fullscreen mode",
                                    cxxopts::value<bool>()->default_value("false"))(
        "vsync", "Enable vsync", cxxopts::value<bool>()->default_value("true"));

    options.add_options("Load")("swc", "Load a SWC file", cxxopts::value<std::vector<std::string>>())(
        "xml", "Load a XML file", cxxopts::value<std::vector<std::string>>())
#ifdef MINDSET_BRION
        ("blueconfig", "Load a BlueConfig file", cxxopts::value<std::vector<std::string>>())(
            "morpho", "Load a Morpho IO file", cxxopts::value<std::vector<std::string>>())
#endif
            ("snudda", "Load a snudda file", cxxopts::value<std::vector<std::string>>())(
                "target", "Configure a BlueConfig target", cxxopts::value<std::vector<std::string>>())(
                "snuddapath", "Configure snudda data path", cxxopts::value<std::string>());

    auto args = options.parse(argc, argv);
    if (args.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    using MeshFeature = VkPhysicalDeviceMeshShaderFeaturesEXT;
    constexpr VkStructureType MESH_FEATURE = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;

    neon::vulkan::VKApplicationCreateInfo info;
    info.name = "Neoneuron";
    info.extraFeatures.emplace_back(MeshFeature{MESH_FEATURE});
    info.icon.push_back(
        neon::TextureData::fromFile(cmrc::resources::get_filesystem().open("icon/16x16/neoneuron.png")));
    info.icon.push_back(
        neon::TextureData::fromFile(cmrc::resources::get_filesystem().open("icon/32x32/neoneuron.png")));
    info.icon.push_back(
        neon::TextureData::fromFile(cmrc::resources::get_filesystem().open("icon/64x64/neoneuron.png")));
    info.icon.push_back(
        neon::TextureData::fromFile(cmrc::resources::get_filesystem().open("icon/128x128/neoneuron.png")));
    info.icon.push_back(
        neon::TextureData::fromFile(cmrc::resources::get_filesystem().open("icon/256x256/neoneuron.png")));
    info.icon.push_back(
        neon::TextureData::fromFile(cmrc::resources::get_filesystem().open("icon/512x512/neoneuron.png")));

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

    info.vSync = args["vsync"].as<bool>();
    info.fullscreen = args["fullscreen"].as<bool>();
    neon::debug() << "Fullscreen: " << info.fullscreen;

    neoneuron::NeoneuronApplication app(info);

    parseLoaderArguments(args, app);

    // app.getRender().setSkybox(loadSkybox(app.getRender().getRoom().get()));

    bool result = app.getRender().renderLoop();
    app.getFiles().saveSettings();
    return result ? 0 : 1;
}

void loadDataset(neoneuron::NeoneuronApplication& app, mindset::Environment env, std::filesystem::path path,
                 const std::string& datasetPrefix, const std::string& loaderId)
{
    if (auto opt = app.getLoaderRegistry().get(loaderId); opt.has_value()) {
        auto fileSystem = std::make_shared<neon::DirectoryFileSystem>(path.parent_path());
        auto fileProvider = [fileSystem](std::filesystem::path p) {
            return fileSystem->readFile(std::move(p))->readLines();
        };
        auto loader = opt.value().create(std::move(fileProvider), std::move(env), path);
        if (loader.isOk()) {
            std::string name = std::format("{} - {}", datasetPrefix, path.filename().string());
            auto [uid, _, ok] = app.getRepository().addDataset(mindset::Dataset(), name);
            auto dataset = app.getRepository().getDatasetAsOwned(uid).value();
            app.getLoaderCollection().load(std::move(loader.getResult()), dataset);
        }
    }
}

void parseLoaderArguments(const cxxopts::ParseResult& result, neoneuron::NeoneuronApplication& app)
{
    std::vector<std::tuple<std::string, std::string, std::string>> LOADERS = {
        {    "snudda",   "Snudda",      mindset::SNUDDA_LOADER_ID},
        {       "swc",      "SWC",         mindset::SWC_LOADER_ID},
        {       "xml",      "XML",         mindset::XML_LOADER_ID},
#ifdef MINDSET_BRION
        {"blueconfig",    "Brion", mindset::BLUE_CONFIG_LOADER_ID},
        {    "morpho", "MorphoIO",   mindset::MORPHO_IO_LOADER_ID},
#endif
    };

    mindset::Environment env;
    if (result.count("snuddapath")) {
        env[mindset::SNUDDA_LOADER_ENTRY_SNUDDA_DATA_PATH] = result["snuddapath"].as<std::string>();
    }

#ifdef MINDSET_BRION
    if (result.count("target")) {
        env[mindset::BLUE_CONFIG_LOADER_ENTRY_TARGETS] = result["target"].as<std::vector<std::string>>();
    }
#endif

    for (auto& [argId, displayName, loaderId] : LOADERS) {
        if (result.count(argId)) {
            for (auto& stringPath : result[argId].as<std::vector<std::string>>()) {
                auto path = std::filesystem::path(stringPath);
                loadDataset(app, env, path, displayName, loaderId);
            }
        }
    }
}
