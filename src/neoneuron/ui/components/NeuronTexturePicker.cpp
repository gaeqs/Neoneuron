// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "NeuronTexturePicker.h"

#include <neon/io/CursorEvent.h>
#include <neon/structure/collection/AssetCollection.h>
#include <neon/io/MouseButtonEvent.h>
#include <neoneuron/render/NeoneuronRender.h>

#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{
    std::vector<std::pair<GID, mindset::UID>> NeuronTexturePicker::pickNeurons(const rush::Vec4i* data, size_t size)
    {
        std::vector<std::pair<GID, mindset::UID>> selection;

        for (size_t i = 0; i < size; ++i) {
            rush::Vec4i current = data[i];
            if (current.x() > 0) {
                mindset::UID datasetId = current.x() - 1;
                mindset::UID neuronId = current.y();
                mindset::UID neuriteId = current.z();
                selection.push_back(std::make_pair(GID(datasetId, neuronId), neuriteId));
            }
        }

        return selection;
    }

    NeuronTexturePicker::NeuronTexturePicker(NeoneuronApplication* application, Viewport* viewport) :
        _application(application),
        _viewport(viewport)
    {
        _texture = _viewport->getPickerTexture();
        if (_texture == nullptr) {
            getLogger().error("Cannot find the texture neoneuron:frame_buffer_picker_resolved!");
        }
    }

    void NeuronTexturePicker::onMouseButton(const neon::MouseButtonEvent& event)
    {
        if (_texture == nullptr || event.button != neon::MouseButton::BUTTON_PRIMARY) {
            return;
        }

        if (event.action == neon::KeyboardAction::PRESS) {
            if (!_inside || event.modifiers != 0) {
                return;
            }
            _selecting = true;
            _origin = _pixelPosition;
        } else if (event.action == neon::KeyboardAction::RELEASE) {
            if (!_selecting) {
                return;
            }

            auto imSize = _viewport->getWindowSize();
            auto winSize = rush::Vec2f(imSize.x, imSize.y).cast<int32_t>() - 1;

            rush::Vec2i to = rush::clamp(_pixelPosition, rush::Vec<2, int32_t>(0, 0), winSize);

            auto min = rush::min(_origin, to);
            auto max = rush::max(_origin, to);
            rush::Vec<3, uint32_t> size = {(max + 1 - min).cast<uint32_t>(), 1};

            uint32_t amount = size[0] * size[1];

            auto data = std::make_unique<rush::Vec4i[]>(amount);
            if (auto read =_texture->get()->getTexture()->asReadable()) {
                read.value()->readData(data.get(), {min.cast<uint32_t>(), 0}, size, 0, 1);
            }

            auto& selector = _application->getSelector();
            selector.selectSegments(SelectionMode::OVERRIDE_ALL, pickNeurons(data.get(), amount));

            _selecting = false;
        }
    }

    void NeuronTexturePicker::onCursorMove(const neon::CursorMoveEvent& event)
    {
        auto origin = _viewport->getWindowOrigin();
        auto size = _viewport->getWindowSize();
        auto rush = rush::Vec2f(origin.x, origin.y);
        _pixelPosition = (event.position - rush).cast<int32_t>();

        _inside = _viewport->isHovered() && _pixelPosition.x() >= 0 && _pixelPosition.y() >= 0 &&
                  _pixelPosition.x() < size.x && _pixelPosition.y() < size.y;
    }

    void NeuronTexturePicker::onPreDraw()
    {
        if (!_selecting) {
            return;
        }
        auto base = rush::Vec2i(_viewport->getWindowOrigin().x, _viewport->getWindowOrigin().y);
        auto size = rush::Vec2i(_viewport->getWindowSize().x, _viewport->getWindowSize().y);
        auto from = base + _origin;
        auto to = base + rush::clamp(_pixelPosition, rush::Vec2i(0), size);

        auto min = rush::min(from, to).cast<float>();
        auto max = rush::max(from, to).cast<float>();
        ImGui::GetForegroundDrawList()->AddRectFilled({min.x(), min.y()}, {max.x(), max.y()}, 0x550000FF, 5.f);
    }
} // namespace neoneuron
