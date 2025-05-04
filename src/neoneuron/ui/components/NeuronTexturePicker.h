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

#ifndef NEURONTEXTUREPICKER_H
#define NEURONTEXTUREPICKER_H

#include <mindset/UID.h>
#include <neon/render/texture/Texture.h>
#include <neon/structure/Component.h>
#include <neoneuron/structure/GID.h>

namespace neoneuron
{
    class NeoneuronApplication;
    class Viewport;

    class NeuronTexturePicker : public neon::Component
    {
        NeoneuronApplication* _application;
        Viewport* _viewport;
        std::shared_ptr<neon::Texture> _texture;

        bool _inside = false;
        rush::Vec2i _pixelPosition;

        bool _selecting = false;
        rush::Vec2i _origin;

        std::vector<std::pair<GID, mindset::UID>> pickNeurons(const rush::Vec4i* data, size_t size);

      public:
        explicit NeuronTexturePicker(NeoneuronApplication* application, Viewport* viewport);

        void onMouseButton(const neon::MouseButtonEvent& event) override;

        void onCursorMove(const neon::CursorMoveEvent& event) override;

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // NEURONTEXTUREPICKER_H
