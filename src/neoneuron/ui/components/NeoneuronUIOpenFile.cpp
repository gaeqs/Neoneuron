//
// Created by gaeqs on 3/01/25.
//

#include "NeoneuronUIOpenFile.h"

#include <neon/util/Chronometer.h>
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
        auto loader = _loaders[_selectedLoader]->build(_fileSystem.get(), _file);
        loader->setPath(_path);
        return loader;
    }

    void NeoneuronUiOpenFile::loaderCombo() {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Loader:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::Combo(
            "##loader",
            &_selectedLoader,
            _loaderNames.data(),
            _loaderNames.size()
        );
    }

    void NeoneuronUiOpenFile::uidProviderCombo() {
        if (_selectedLoader < 0 || _selectedLoader >= _loaders.size()) return;
        if (!_loaders[_selectedLoader]->providesUIDs()) return;

        ImGui::AlignTextToFramePadding();
        ImGui::Text("UID Provider:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

        int index = static_cast<int>(_uidProvider);

        ImGui::Combo(
            "##uid_provider",
            &index,
            "Provided by the file\0Generate automatically\0"
        );

        _uidProvider = static_cast<UIDProvider>(index);

        if (_uidProvider == UIDProvider::FILE && _scene->getNeuronsAmount() > 0) {
            ImGui::Indent();
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
            ImGui::TextWrapped(
                "Neurons provided by the file may overwrite the neurons present in the scene if the share the same UID.");
            ImGui::PopStyleColor();
            ImGui::Unindent();
        }
    }

    void NeoneuronUiOpenFile::loadButton() const {
        ImGui::BeginDisabled(_selectedLoader < 0 || _selectedLoader >= _loaders.size());
        ImGui::Separator();
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 40.0f);
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 140.0f);
        if (ImGui::Button("Load", ImVec2(120, 0))) {
            neon::Chronometer chrono;

            if (auto result = generateLoader()->build(); result.isOk()) {
                for (auto& prototype: result.getResult()) {
                    changeNeuronUID(prototype);
                    _scene->addNeuron(std::move(prototype));
                }
                _scene->getRender()->focusScene();
            } else {
                getLogger().error(result.getError());
            }
            ImGui::CloseCurrentPopup();
            getLogger().debug(neon::MessageBuilder()
                .print("Time required to load: ")
                .print(chrono.elapsedSeconds())
                .print(" seconds."));
        }
        ImGui::EndDisabled();
    }

    void NeoneuronUiOpenFile::changeNeuronUID(PrototypeNeuron& neuron) const {
        auto& loader = _loaders[_selectedLoader];
        if (loader->providesUIDs() && _uidProvider == UIDProvider::FILE) return;
        neuron.setId(_scene->findAvailableUID());
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
          _selectedLoader(0),
          _uidProvider(UIDProvider::FILE) {
        auto& loaders = scene->getRender()->getNeoneuronApplication()->getLoaderStorage();
        for (auto& loader: loaders | std::views::values) {
            _loaders.push_back(&loader);
            _loaderNames.push_back(loader.getDisplayName().c_str());
        }

        _selectedLoader = fetchLoaderIndex(fetchBestLoaderName());
    }

    void NeoneuronUiOpenFile::onPreDraw() {
        if (!_open) {
            ImGui::OpenPopup("Open file");
            _open = true;
        }
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 150), ImVec2(1000, 1000));
        if (ImGui::BeginPopupModal("Open file", &_open)) {
            loaderCombo();
            uidProviderCombo();
            loadButton();

            ImGui::EndPopup();
        }

        if (!_open) {
            getGameObject()->destroy();
        }
    }
}
