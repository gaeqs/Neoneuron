//
// Created by gaeqs on 23/10/2024.
//

#include "Guide.h"

#include <neoneuron/render/NeoneuronRender.h>

CMRC_DECLARE(resources);

namespace neoneuron {
    void Guide::updateState(bool active) {
        _model->getInstanceData(0)->uploadData(
            _instance, 0, GuideInstancingData{
                active ? 1.0f : 0.0f,
                _render->getCurrentTime()
            }
        );
    }

    Guide::Guide(NeoneuronRender* render)
        : _render(render),
          _positionListener([this](bool active) {
              updateState(active);
          }) {}

    Guide::~Guide() {
        if (_model != nullptr) {
            getRoom()->unmarkUsingModel(_model.get());
        }
    }

    void Guide::onStart() {
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;
        _model = neon::loadAssetFromFile<neon::Model>("/model/guide/guide.json", context);

        getRoom()->markUsingModel(_model.get());

        _instance = _model->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _render->getCameraData().onActivePosition() += _positionListener;

        updateState(false);
    }
}
