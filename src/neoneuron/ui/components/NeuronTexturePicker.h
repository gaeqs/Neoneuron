//
// Created by gaeqs on 13/11/2024.
//

#ifndef NEURONTEXTUREPICKER_H
#define NEURONTEXTUREPICKER_H

#include <mindset/UID.h>
#include <neon/render/texture/Texture.h>
#include <neon/structure/Component.h>
#include <neon/util/component/ViewportComponent.h>
#include <neoneuron/structure/GID.h>

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

        std::vector<std::pair<GID, mindset::UID>> pickNeurons(const rush::Vec4i* data, size_t size);

      public:
        explicit NeuronTexturePicker(NeoneuronRender* render,
                                     neon::IdentifiableWrapper<neon::ViewportComponent> viewport);

        void onStart() override;

        void onMouseButton(const neon::MouseButtonEvent& event) override;

        void onCursorMove(const neon::CursorMoveEvent& event) override;

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // NEURONTEXTUREPICKER_H
