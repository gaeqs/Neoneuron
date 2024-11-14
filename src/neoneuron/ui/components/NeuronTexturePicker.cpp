//
// Created by gaeqs on 13/11/2024.
//

#include "NeuronTexturePicker.h"

#include <neon/io/CursorEvent.h>
#include <neon/structure/collection/AssetCollection.h>
#include <neon/io/MouseButtonEvent.h>


namespace neoneuron {
    std::optional<std::pair<UID, UID>> NeuronTexturePicker::pickNeuron(rush::Vec2i min,
                                                                       rush::Vec2i max,
                                                                       const rush::Vec4i* data) {
        int32_t squaredLength = std::numeric_limits<float>::max();
        std::optional<std::pair<UID, UID>> selected = {};

        size_t i = 0;
        for (int32_t y = min.y(); y <= max.y(); ++y) {
            for (int32_t x = min.x(); x <= max.x(); ++x) {
                rush::Vec4i current = data[i++];
                int32_t currentLength = x * x + y * y;
                if (current.x() > 0 && (!selected.has_value() || currentLength < squaredLength)) {
                    squaredLength = currentLength;
                    selected = {current.y(), current.z()};
                }
            }
        }

        return selected;
    }

    NeuronTexturePicker::NeuronTexturePicker(neon::IdentifiableWrapper<neon::ViewportComponent> viewport)
        : _viewport(viewport) {}

    void NeuronTexturePicker::onStart() {
        _texture = getAssets().get<neon::Texture>("neoneuron:frame_buffer_picker").value_or(nullptr);
        if (_texture == nullptr) {
            getLogger().error("Cannot find the texture neoneuron:frame_buffer_picker!");
        }
    }

    void NeuronTexturePicker::onMouseButton(const neon::MouseButtonEvent& event) {
        constexpr size_t RADIUS = 5;

        if (_texture == nullptr ||
            !_inside ||
            event.button != neon::MouseButton::BUTTON_PRIMARY ||
            event.action != neon::KeyboardAction::PRESS)
            return;


        rush::Vec2i min = rush::max(_pixelPosition - RADIUS, 0);
        rush::Vec2i max = rush::min(_pixelPosition + RADIUS,
                                    rush::Vec2i(_texture->getWidth() - 1, _texture->getHeight() - 1));
        rush::Vec<3, uint32_t> size = {(max + 1 - min).cast<uint32_t>(), 1};
        uint32_t amount = size[0] * size[1];

        auto data = std::make_unique<rush::Vec4i[]>(amount);
        _texture->fetchData(data.get(), {min, 0}, size, 0, 1);

        auto result = pickNeuron(min, max, data.get());

        if (result.has_value()) {
            getLogger().debug(neon::MessageBuilder()
                .print("Segment found! ")
                .print(result.value().first)
                .print(" - ")
                .print(result.value().second));
        }
    }

    void NeuronTexturePicker::onCursorMove(const neon::CursorMoveEvent& event) {
        auto origin = _viewport->getWindowOrigin();
        auto size = _viewport->getWindowSize();
        auto rush = rush::Vec2f(origin.x, origin.y);
        _pixelPosition = (event.position - rush).cast<int32_t>();

        _inside = _pixelPosition.x() >= 0 && _pixelPosition.y() >= 0 &&
                  _pixelPosition.x() < size.x && _pixelPosition.y() < size.y;
    }
}
