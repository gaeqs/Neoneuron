//
// Created by gaeqs on 13/11/2024.
//

#ifndef NEURONTEXTUREPICKER_H
#define NEURONTEXTUREPICKER_H

#include <unordered_set>
#include <neon/render/texture/Texture.h>
#include <neon/structure/Component.h>
#include <neon/util/component/ViewportComponent.h>
#include <neoneuron/Types.h>

namespace neoneuron
{
    class NeoneuronRender;

    class NeuronTexturePicker : public neon::Component
    {
        NeoneuronRender* _render;
        std::shared_ptr<neon::Texture> _texture;
        neon::IdentifiableWrapper<neon::ViewportComponent> _viewport;

        bool _inside = false;
        rush::Vec2i _pixelPosition;

        bool _selecting = false;
        rush::Vec2i _origin;

        std::unordered_set<rush::Vec<2, uint32_t>> pickNeurons(const rush::Vec4i* data, size_t size);

      public:
        explicit NeuronTexturePicker(NeoneuronRender* render,
                                     neon::IdentifiableWrapper<neon::ViewportComponent> viewport);

        void onStart() override;

        void onMouseButton(const neon::MouseButtonEvent& event) override;

        void onCursorMove(const neon::CursorMoveEvent& event) override;

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif //NEURONTEXTUREPICKER_H
