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
        uint32_t minChildrenForJoint = 2;
        uint32_t verticesPerCircle = 16;
        uint32_t somaLatitudes = 32;
        uint32_t somaLongitudes = 32;
        float somaConnectionPushFactor = 0.3f;
        float somaWeightPower = 3.0f;
        float somaSphereWeight = 0.1f;
        float somaConnectionMaxWeight = 1.0f;
        rush::Vec4f defaultColor = rush::Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
        rush::Vec4f selectedColor = rush::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
        uint32_t lod = 0;
        uint32_t frame = 0;
    };
}
#endif //NEONEURONRENDERDATA_H
