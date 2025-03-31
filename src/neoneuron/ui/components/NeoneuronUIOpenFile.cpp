//
// Created by gaeqs on 3/01/25.
//

#include "NeoneuronUIOpenFile.h"

#include <mindset/loader/BlueConfigLoader.h>
#include <mindset/loader/SWCLoader.h>
#include <neon/util/Chronometer.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>

namespace neoneuron
{
    std::string NeoneuronUiOpenFile::fetchBestLoaderName() const
    {
        auto name = _path.filename().string();
        for (auto& loader : _loaders) {
            if (loader.supportsFile(name)) {
                return loader.getDisplayName();
            }
        }

        return mindset::SWC_LOADER_NAME;
    }

    int NeoneuronUiOpenFile::fetchLoaderIndex(const std::string& name) const
    {
        for (int i = 0; i < _loaders.size(); ++i) {
            if (_loaders[i].getDisplayName() == name) {
                return i;
            }
        }
        return -1;
    }

    std::unique_ptr<mindset::Loader> NeoneuronUiOpenFile::generateLoader() const
    {
        auto fileProvider = [&](std::filesystem::path path) { return _fileSystem->readFile(path)->readLines(); };

        auto loader = _loaders[_selectedLoader].create(fileProvider, _path);
        if (!loader.isOk()) {
            std::cerr << loader.getError() << std::endl;
            return nullptr;
        }
        return std::move(loader.getResult());
    }

    void NeoneuronUiOpenFile::loaderCombo()
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Loader:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::Combo("##loader", &_selectedLoader, _loaderCNames.data(), _loaderCNames.size());
    }

    void NeoneuronUiOpenFile::uidProviderCombo()
    {
        if (_selectedLoader < 0 || _selectedLoader >= _loaders.size()) {
            return;
        }
        if (!_loaders[_selectedLoader].providesUIDs()) {
            return;
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text("UID Provider:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

        int index = static_cast<int>(_uidProvider);

        ImGui::Combo("##uid_provider", &index, "Provided by the file\0Generate automatically\0");

        _uidProvider = static_cast<UIDProvider>(index);

        if (_uidProvider == UIDProvider::FILE && _scene->getNeuronsAmount() > 0) {
            ImGui::Indent();
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
            ImGui::TextWrapped("Neurons provided by the file may overwrite the neurons present in the scene if the "
                               "share the same UID.");
            ImGui::PopStyleColor();
            ImGui::Unindent();
        }
    }

    void NeoneuronUiOpenFile::loadButton() const
    {
        ImGui::BeginDisabled(_selectedLoader < 0 || _selectedLoader >= _loaders.size());
        ImGui::Separator();
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 40.0f);
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 140.0f);
        if (ImGui::Button("Load", ImVec2(120, 0))) {
            neon::Chronometer chrono;
            auto loader = generateLoader();
            loader->addUIDProvider([&] { return _scene->findAvailableUID(); });

            if (auto* l = dynamic_cast<mindset::BlueConfigLoader*>(loader.get())) {
                l->addTarget("MiniColumn_501");
            }

            auto listener = loader->createListener([](mindset::LoaderStatus status) {
                std::cout << status.currentTask << " (" << status.stagesCompleted << "/" << status.stages << ")"
                          << std::endl;
            });

            auto& dataset = _scene->getRender()->getNeoneuronApplication()->getDataset();
            loader->load(dataset);
            neon::debug() << chrono.elapsedSeconds() << " - Scene loaded. Refreshing scene.";

            ImGui::CloseCurrentPopup();
            getLogger().debug(neon::MessageBuilder()
                                  .print("Time required to load: ")
                                  .print(chrono.elapsedSeconds())
                                  .print(" seconds."));

            neon::debug() << "Neurons in dataset: " << dataset.getNeurons().size();
            neon::debug() << "Synapses in dataset: " << dataset.getCircuit().getSynapses().size();
        }
        ImGui::EndDisabled();
    }

    NeoneuronUiOpenFile::NeoneuronUiOpenFile(AbstractNeuronRepresentation* scene,
                                             std::unique_ptr<neon::FileSystem> fileSystem, std::filesystem::path path,
                                             neon::File file) :
        _scene(scene),
        _fileSystem(std::move(fileSystem)),
        _path(std::move(path)),
        _file(std::move(file)),
        _open(false),
        _selectedLoader(0),
        _uidProvider(UIDProvider::FILE)
    {
        auto& loaders = scene->getRender()->getNeoneuronApplication()->getLoaderRegistry();
        for (auto& loader : loaders.getAll()) {
            _loaders.push_back(loader);
            _loaderNames.push_back(loader.getDisplayName());
        }

        for (auto& loader : _loaderNames) {
            _loaderCNames.push_back(loader.c_str());
        }

        _selectedLoader = fetchLoaderIndex(fetchBestLoaderName());
    }

    void NeoneuronUiOpenFile::onPreDraw()
    {
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
} // namespace neoneuron
