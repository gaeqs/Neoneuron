//
// Created by gaeqs on 29/11/24.
//

#ifndef NEONEURONRENDERDATA_H
#define NEONEURONRENDERDATA_H

namespace neoneuron {
    struct NeoneuronRenderData {
        float currentTime = 0.0f; // Value only valid on GPU. use Render::getCurrentTime() instead.
        float radiusStrength = 1.0f;
        uint32_t rotationIndexOffset = 0;
        uint32_t childrenRotationIndexOffset = 0;
    };
}
#endif //NEONEURONRENDERDATA_H
