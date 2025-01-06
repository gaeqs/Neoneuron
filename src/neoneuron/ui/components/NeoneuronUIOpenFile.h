//
// Created by gaeqs on 3/01/25.
//

#ifndef NEONEURONUIOPENFILE_H
#define NEONEURONUIOPENFILE_H

#include <neon/Neon.h>
#include <neoneuron/loader/Loader.h>
#include <neoneuron/render/AbstractNeuronScene.h>

namespace neoneuron {
    class NeoneuronUiOpenFile : public neon::Component {
        AbstractNeuronScene* _scene;
        std::unique_ptr<neon::FileSystem> _fileSystem;
        std::filesystem::path _path;
        neon::File _file;
        bool _open;

        std::vector<LoaderBuilder*> _loaders;
        std::vector<const char*> _names;
        int _selected;

        std::string fetchBestLoaderName() const;

        int fetchLoaderIndex(const std::string& name) const;

        std::unique_ptr<Loader> generateLoader() const;

        void loadButton() const;

    public:
        explicit NeoneuronUiOpenFile(AbstractNeuronScene* scene,
                                     std::unique_ptr<neon::FileSystem> fileSystem,
                                     std::filesystem::path path,
                                     neon::File file);

        void onPreDraw() override;
    };
}

#endif //NEONEURONUIOPENFILE_H
