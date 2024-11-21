//
// Created by gaeqs on 13/11/2024.
//

#include "NeuronTexturePicker.h"

#include <neon/io/CursorEvent.h>
#include <neon/structure/collection/AssetCollection.h>
#include <neon/io/MouseButtonEvent.h>
#include <neoneuron/render/NeoneuronRender.h>

#include "nfd_glfw3.h"


namespace neoneuron {
    std::unordered_set<rush::Vec<2, uint32_t>>
    NeuronTexturePicker::pickNeurons(const rush::Vec4i* data, size_t size) {
        std::unordered_set<rush::Vec<2, uint32_t>> selection;

        for (size_t i = 0; i < size; ++i) {
            rush::Vec4i current = data[i];
            if (current.x() > 0) {
                rush::Vec<2, uint32_t> pair = {current.y(), current.z()};
                selection.insert(pair);
            }
        }

        return selection;
    }

    NeuronTexturePicker::NeuronTexturePicker(NeoneuronRender* render,
                                             neon::IdentifiableWrapper<neon::ViewportComponent> viewport)
        : _render(render), _viewport(viewport) {}

    void NeuronTexturePicker::onStart() {
        _texture = getAssets().get<neon::Texture>("neoneuron:frame_buffer_picker").value_or(nullptr);
        if (_texture == nullptr) {
            getLogger().error("Cannot find the texture neoneuron:frame_buffer_picker!");
        }
    }

    void NeuronTexturePicker::onMouseButton(const neon::MouseButtonEvent& event) {
        if (_texture == nullptr || event.button != neon::MouseButton::BUTTON_PRIMARY) return;

        if (event.action == neon::KeyboardAction::PRESS) {
            if (!_inside || event.modifiers != 0) return;
            _selecting = true;
            _origin = _pixelPosition;
        } else if (event.action == neon::KeyboardAction::RELEASE) {
            if (!_selecting) return;

            auto imSize = _viewport->getWindowSize();
            auto winSize = rush::Vec2f(imSize.x, imSize.y).cast<int32_t>() - 1;

            rush::Vec2i to = rush::clamp(_pixelPosition, rush::Vec<2, int32_t>(0, 0), winSize);

            auto min = rush::min(_origin, to);
            auto max = rush::max(_origin, to);
            rush::Vec<3, uint32_t> size = {(max + 1 - min).cast<uint32_t>(), 1};

            uint32_t amount = size[0] * size[1];

            auto data = std::make_unique<rush::Vec4i[]>(amount);
            _texture->fetchData(data.get(), {min, 0}, size, 0, 1);

            auto result = pickNeurons(data.get(), amount);

            auto& selector = _render->getNeuronScene()->getSelector();

            Selection selection(true);

            selection.selections.insert(
                selection.selections.begin(),
                result.begin(),
                result.end()
            );

            selector.setSelectionData(selection);

            _selecting = false;
        }
    }

    void NeuronTexturePicker::onCursorMove(const neon::CursorMoveEvent& event) {
        auto origin = _viewport->getWindowOrigin();
        auto size = _viewport->getWindowSize();
        auto rush = rush::Vec2f(origin.x, origin.y);
        _pixelPosition = (event.position - rush).cast<int32_t>();

        _inside = _viewport->isHovered() &&
                  _pixelPosition.x() >= 0 && _pixelPosition.y() >= 0 &&
                  _pixelPosition.x() < size.x && _pixelPosition.y() < size.y;
    }

    void NeuronTexturePicker::onPreDraw() {
        if (!_selecting) return;
        auto base = rush::Vec2i(_viewport->getWindowOrigin().x, _viewport->getWindowOrigin().y);
        auto size = rush::Vec2i(_viewport->getWindowSize().x, _viewport->getWindowSize().y);
        auto from = base + _origin;
        auto to = base + rush::clamp(_pixelPosition, rush::Vec2i(0), size);

        auto min = rush::min(from, to).cast<float>();
        auto max = rush::max(from, to).cast<float>();
        ImGui::GetForegroundDrawList()->AddRectFilled(
            {min.x(), min.y()},
            {max.x(), max.y()},
            0x550000FF,
            5.f
        );
    }
}
