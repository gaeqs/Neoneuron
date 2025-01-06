//
// Created by gaeqs on 3/01/25.
//

#include "NeoneuronUIOpenFile.h"

#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/loader/SWCLoader.h>
#include <neoneuron/loader/XMLLoader.h>
#include <neoneuron/render/NeoneuronRender.h>


namespace neoneuron {
    std::string NeoneuronUiOpenFile::fetchBestLoaderName() const {
        auto extension = _path.extension().string();
        std::ranges::transform(extension, extension.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        if (extension == ".xml") return XML_LOADER_NAME;
        return SWC_LOADER_NAME;
    }

    int NeoneuronUiOpenFile::fetchLoaderIndex(const std::string& name) const {
        for (int i = 0; i < _loaders.size(); ++i) {
            if (_loaders[i]->getName() == name) return i;
        }
        return -1;
    }

    std::unique_ptr<Loader> NeoneuronUiOpenFile::generateLoader() const {
        return _loaders[_selected]->build(_fileSystem.get(), _file);
    }

    void NeoneuronUiOpenFile::loadButton() const {
        ImGui::BeginDisabled(_selected < 0 || _selected >= _loaders.size());
        ImGui::Separator();
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 40.0f);
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 140.0f);
        if (ImGui::Button("Load", ImVec2(120, 0))) {
            if (auto result = generateLoader()->build(_scene->getNeuronsAmount()); result.isOk()) {
                for (auto& prototype: result.getResult()) {
                    _scene->addNeuron(std::move(prototype));
                }
            } else {
                getLogger().error(result.getError());
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();
    }

    NeoneuronUiOpenFile::NeoneuronUiOpenFile(AbstractNeuronScene* scene,
                                             std::unique_ptr<neon::FileSystem> fileSystem,
                                             std::filesystem::path path,
                                             neon::File file)
        : _scene(scene),
          _fileSystem(std::move(fileSystem)),
          _path(std::move(path)),
          _file(std::move(file)),
          _open(false),
          _selected(0) {
        auto& loaders = scene->getRender()->getNeoneuronApplication()->getLoaderStorage();
        for (auto& loader: loaders | std::views::values) {
            _loaders.push_back(&loader);
            _names.push_back(loader.getDisplayName().c_str());
        }

        _selected = fetchLoaderIndex(fetchBestLoaderName());
    }

    void NeoneuronUiOpenFile::onPreDraw() {
        if (!_open) {
            ImGui::OpenPopup("Open file");
            _open = true;
        }
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 150), ImVec2(1000, 1000));
        if (ImGui::BeginPopupModal("Open file", &_open)) {
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Loader:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Combo("##loader", &_selected, _names.data(), _names.size());

            loadButton();

            ImGui::EndPopup();
        }

        if (!_open) {
            getGameObject()->destroy();
        }
    }
}
