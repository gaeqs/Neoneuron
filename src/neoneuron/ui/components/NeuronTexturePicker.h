//
// Created by gaeqs on 13/11/2024.
//

#ifndef NEURONTEXTUREPICKER_H
#define NEURONTEXTUREPICKER_H
#include <neon/render/texture/Texture.h>
#include <neon/structure/Component.h>
#include <neon/util/component/ViewportComponent.h>
#include <neoneuron/Types.h>

namespace neoneuron {
    class NeuronTexturePicker : public neon::Component {
        std::shared_ptr<neon::Texture> _texture;
        neon::IdentifiableWrapper<neon::ViewportComponent> _viewport;

        bool _inside = false;
        rush::Vec2i _pixelPosition;

        std::optional<std::pair<UID, UID>> pickNeuron(rush::Vec2i min, rush::Vec2i max, const rush::Vec4i* data);

    public:
        explicit NeuronTexturePicker(neon::IdentifiableWrapper<neon::ViewportComponent> viewport);

        void onStart() override;

        void onMouseButton(const neon::MouseButtonEvent& event) override;

        void onCursorMove(const neon::CursorMoveEvent& event) override;
    };
}


#endif //NEURONTEXTUREPICKER_H
