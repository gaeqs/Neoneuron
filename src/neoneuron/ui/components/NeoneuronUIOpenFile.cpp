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

#include "NeoneuronUIOpenFile.h"

#include "neon/util/dialog/Dialogs.h"

#include <mindset/loader/BlueConfigLoader.h>
#include <mindset/loader/SWCLoader.h>
#include <neon/util/Chronometer.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/ui/editor/Editors.h>

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

    std::shared_ptr<NamedDataset> NeoneuronUiOpenFile::fetchDataset() const
    {
        if (_selectedDataset > 0 && _selectedDataset < _datasetNames.size()) {
            auto datasetOptional =
                _application->getRepository().getDatasetAsOwned(_datasetNames[_selectedDataset].first);
            if (datasetOptional.has_value()) {
                return std::move(datasetOptional.value());
            }
            // Fallback and create a dataset.
        }

        std::string name(_newDatasetName);
        if (name.empty()) {
            name = "New dataset";
        }

        auto [uid, dataset, ok] = _application->getRepository().addDataset(mindset::Dataset(), name);
        if (ok) {
            return _application->getRepository().getDatasetAsOwned(uid).value();
        }
        return nullptr;
    }

    std::unique_ptr<mindset::Loader> NeoneuronUiOpenFile::generateLoader() const
    {
        auto fileProvider = [&](std::filesystem::path path) { return _fileSystem->readFile(path)->readLines(); };

        auto loader = _loaders[_selectedLoader].create(fileProvider, _environment, _path);
        if (!loader.isOk()) {
            std::cerr << loader.getError() << std::endl;
            return nullptr;
        }
        return std::move(loader.getResult());
    }

    void NeoneuronUiOpenFile::regenerateEnvironment()
    {
        if (_selectedLoader < 0 || _selectedLoader >= _loaders.size()) {
            return;
        }
        auto& factory = _loaders[_selectedLoader];
        _environment.clear();
        for (auto& [name, entry] : factory.getEnvironmentEntries()) {
            if (entry.defaultValue.has_value()) {
                _environment.insert({name, entry.defaultValue.value()});
            } else {
                if (entry.type == typeid(std::string)) {
                    _environment.insert({name, std::string()});
                }
            }
        }
    }

    void NeoneuronUiOpenFile::loaderCombo()
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Loader:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Combo("##loader", &_selectedLoader, _loaderCNames.data(), _loaderCNames.size())) {
            regenerateEnvironment();
        }
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
    }

    void NeoneuronUiOpenFile::datasetEditor()
    {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::Combo("##dataset", &_selectedDataset, _datasetCNames.data(), _datasetCNames.size());
        if (_selectedDataset == 0) {
            ImGui::Text("Name:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##name", _newDatasetName, MAX_DATASET_NAME_LENGTH);
            ImGui::TextWrapped("The imported data will be saved in a brand-new dataset, fully isolated from any "
                               "existing datasets to prevent ID collisions.");
            return;
        }
        // Fetch dataset
        if (_selectedDataset < 0 || _selectedDataset >= _datasetNames.size()) {
            return;
        }
        auto datasetOptional = _application->getRepository().getDataset(_datasetNames[_selectedDataset].first);
        if (!datasetOptional.has_value()) {
            return;
        }

        auto dataset = &datasetOptional.value()->getDataset();

        ImGui::Text("Dataset information:");
        ImGui::BulletText("Neurons: %d", dataset->getNeurons().size());
        ImGui::BulletText("Synapses: %d", dataset->getCircuit().getSynapses().size());
        ImGui::BulletText("Activities: %d", dataset->getActivities().size());
        ImGui::Separator();
        ImGui::TextWrapped("The imported data will be merged into the selected dataset, updating its records with the "
                           "newly loaded information.");
    }

    void NeoneuronUiOpenFile::environmentEditor()
    {
        if (_selectedLoader < 0 || _selectedLoader >= _loaders.size()) {
            return;
        }

        auto& factory = _loaders[_selectedLoader];

        size_t i = 0;
        for (auto& [name, entry] : factory.getEnvironmentEntries()) {
            auto it = _environment.find(name);
            if (it == _environment.end()) {
                continue;
            }
            editorFor(entry.displayName, &it->second, entry.hint);
            ++i;
        }

        if (i == 0) {
            static const std::string text = "There are no environment variables for this loader.";
            ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
            ImVec2 windowSize = ImGui::GetWindowSize();
            float textPosX = (windowSize.x - textSize.x) * 0.5f;
            float textPosY = (windowSize.y - textSize.y) * 0.5f;

            ImGui::SetCursorPosX(textPosX);
            ImGui::SetCursorPosY(textPosY);
            ImGui::TextDisabled(text.c_str());
        }
    }

    void NeoneuronUiOpenFile::loadButton() const
    {
        ImGui::Dummy(ImVec2(1.0f, ImGui::GetFrameHeightWithSpacing() / 2.0f));

        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 120.0f);
        ImGui::BeginDisabled(_selectedLoader < 0 || _selectedLoader >= _loaders.size());
        if (ImGui::Button("Load", ImVec2(120, 0))) {
            auto loader = generateLoader();
            auto dataset = fetchDataset();
            _application->getLoaderCollection().load(std::move(loader), std::move(dataset));
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();
    }

    NeoneuronUiOpenFile::NeoneuronUiOpenFile(NeoneuronApplication* application,
                                             std::unique_ptr<neon::FileSystem> fileSystem, std::filesystem::path path,
                                             neon::File file) :
        _application(application),
        _fileSystem(std::move(fileSystem)),
        _path(std::move(path)),
        _file(std::move(file)),
        _open(false),
        _selectedLoader(0),
        _selectedDataset(0),
        _newDatasetName("New dataset"),
        _uidProvider(UIDProvider::FILE)
    {
        auto& loaders = _application->getLoaderRegistry();
        for (auto& loader : loaders.getAll()) {
            _loaders.push_back(loader);
            _loaderNames.push_back(loader.getDisplayName());
        }

        for (auto& loader : _loaderNames) {
            _loaderCNames.push_back(loader.c_str());
        }

        _datasetNames.emplace_back(-1, "New Dataset");
        for (auto [uid, dataset] : _application->getRepository().getDatasets()) {
            _datasetNames.emplace_back(uid, dataset->getName());
        }

        for (auto& name : _datasetNames | std::views::values) {
            _datasetCNames.push_back(name.c_str());
        }

        _selectedLoader = fetchLoaderIndex(fetchBestLoaderName());
        regenerateEnvironment();
    }

    void NeoneuronUiOpenFile::onPreDraw()
    {
        if (!_open) {
            ImGui::OpenPopup("Open file");
            _open = true;
        }
        ImGui::SetNextWindowSizeConstraints(ImVec2(600, 300), ImVec2(1000, 750));
        if (ImGui::BeginPopupModal("Open file", &_open)) {
            loaderCombo();

            auto childSize1 = ImGui::GetContentRegionAvail();
            childSize1.y -= ImGui::GetFrameHeightWithSpacing() * 2.0f;
            childSize1.x = childSize1.x * 0.3f - ImGui::GetStyle().ItemInnerSpacing.x;

            auto childSize2 = ImGui::GetContentRegionAvail();
            childSize2.y -= ImGui::GetFrameHeightWithSpacing() * 2.0f;
            childSize2.x = childSize2.x * 0.7f - ImGui::GetStyle().ItemInnerSpacing.x;

            if (ImGui::BeginChild("child1", childSize1, ImGuiChildFlags_AlwaysUseWindowPadding)) {
                datasetEditor();
            }
            ImGui::EndChild();
            ImGui::SameLine();
            if (ImGui::BeginChild("child2", childSize2, true)) {
                uidProviderCombo();
                environmentEditor();
            }
            ImGui::EndChild();

            loadButton();

            ImGui::EndPopup();
        }

        if (!_open) {
            getGameObject()->destroy();
        }
    }

    void NeoneuronUiOpenFile::openDialog(NeoneuronApplication* app)
    {
        auto& runner = app->getRender().getApplication().getTaskRunner();

        auto task = runner.executeAsync(
            [](NeoneuronApplication* nn) -> std::optional<std::filesystem::path> {
                auto& app = nn->getRender().getApplication();
                neon::OpenFileDialogInfo info;
                info.application = &app;

                app.setModalMode(true);
                auto result = neon::openFileDialog(info);
                app.setModalMode(false);

                if (result.empty()) {
                    return {};
                }

                std::filesystem::path path = result[0];
                auto fileSystem = std::make_unique<neon::DirectoryFileSystem>(path.parent_path());
                if (!fileSystem->exists(path.filename())) {
                    return {};
                }
                return path;
            },
            app);

        task->then(
            false, true,
            [task](NeoneuronApplication* app) {
                auto& optional = task->getResult();
                if (!optional.has_value() || !optional->has_value()) {
                    return;
                }
                auto path = std::move(**optional);
                auto fileSystem = std::make_unique<neon::DirectoryFileSystem>(path.parent_path());

                auto file = fileSystem->readFile(path);
                if (!file) {
                    return;
                }

                auto go = app->getRender().getRoom()->newGameObject();
                go->newComponent<NeoneuronUiOpenFile>(app, std::move(fileSystem), path, std::move(*file));
            },
            app);
    }
} // namespace neoneuron
