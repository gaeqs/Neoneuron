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

#ifndef NEONEURONUIOPENFILE_H
#define NEONEURONUIOPENFILE_H

#include <mindset/mindset.h>
#include <neon/Neon.h>
#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{
    class NeoneuronUiOpenFile : public neon::Component
    {
        constexpr static int MAX_DATASET_NAME_LENGTH = 1024;

        enum class UIDProvider
        {
            FILE,
            AVAILABLE
        };

        NeoneuronApplication* _application;
        std::unique_ptr<neon::FileSystem> _fileSystem;
        std::filesystem::path _path;
        neon::File _file;
        bool _open;

        std::vector<mindset::LoaderFactory> _loaders;
        std::vector<std::string> _loaderNames;
        std::vector<const char*> _loaderCNames;

        std::vector<std::pair<mindset::UID, std::string>> _datasetNames;
        std::vector<const char*> _datasetCNames;

        int _selectedLoader;
        int _selectedDataset;

        char _newDatasetName[MAX_DATASET_NAME_LENGTH];

        mindset::Environment _environment;

        UIDProvider _uidProvider;

        [[nodiscard]] std::string fetchBestLoaderName() const;

        [[nodiscard]] int fetchLoaderIndex(const std::string& name) const;

        [[nodiscard]] std::shared_ptr<NamedDataset> fetchDataset() const;

        [[nodiscard]] std::unique_ptr<mindset::Loader> generateLoader() const;

        void regenerateEnvironment();

        void loaderCombo();

        void uidProviderCombo();

        void datasetEditor();

        void environmentEditor();

        void loadButton() const;

      public:
        explicit NeoneuronUiOpenFile(NeoneuronApplication* application, std::unique_ptr<neon::FileSystem> fileSystem,
                                     std::filesystem::path path, neon::File file);

        void onPreDraw() override;

        static void openDialog(NeoneuronApplication* app);
    };
} // namespace neoneuron

#endif // NEONEURONUIOPENFILE_H
