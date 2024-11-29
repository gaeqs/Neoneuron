//
// Created by gaeqs on 29/11/24.
//

#ifndef NEONEURONRENDERDATA_H
#define NEONEURONRENDERDATA_H

namespace neoneuron {
    struct NeoneuronRenderData {
        float currentTime = 0.0f; // Value only valid on GPU. use Render::getCurrentTime() instead.
        float radiusStrength = 1.0f;
        float startClip = 0.2f;
        float endClip = 0.1f;
        float splitHeight = 0.05f;
        float splitArcStrength = 0.15f;
        uint32_t rotationIndexOffset = 0;
        uint32_t childrenRotationIndexOffset = 0;
    };
}
#endif //NEONEURONRENDERDATA_H
