//
// Created by gaeqs on 3/01/25.
//

#ifndef NEONEURONUIOPENFILE_H
#define NEONEURONUIOPENFILE_H

#include <neon/Neon.h>
#include <mindset/loader/Loader.h>
#include <neoneuron/render/AbstractNeuronRepresentation.h>

namespace neoneuron
{
    class NeoneuronUiOpenFile : public neon::Component
    {
        enum class UIDProvider
        {
            FILE,
            AVAILABLE
        };

        AbstractNeuronRepresentation* _scene;
        std::unique_ptr<neon::FileSystem> _fileSystem;
        std::filesystem::path _path;
        neon::File _file;
        bool _open;

        std::vector<mindset::LoaderFactory> _loaders;
        std::vector<std::string> _loaderNames;
        std::vector<const char*> _loaderCNames;
        int _selectedLoader;

        UIDProvider _uidProvider;

        [[nodiscard]] std::string fetchBestLoaderName() const;

        [[nodiscard]] int fetchLoaderIndex(const std::string& name) const;

        [[nodiscard]] std::unique_ptr<mindset::Loader> generateLoader() const;

        void loaderCombo();

        void uidProviderCombo();

        void loadButton() const;

      public:
        explicit NeoneuronUiOpenFile(AbstractNeuronRepresentation* scene, std::unique_ptr<neon::FileSystem> fileSystem,
                                     std::filesystem::path path, neon::File file);

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif //NEONEURONUIOPENFILE_H
