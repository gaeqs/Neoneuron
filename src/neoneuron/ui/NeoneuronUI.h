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

#ifndef NEONEURONUI_H
#define NEONEURONUI_H

#include <neoneuron/ui/style/DynamicColorPalette.h>

#include <hey/Listener.h>
#include <neon/Neon.h>

namespace neoneuron
{
    class NeoneuronRender;

    class NeoneuronUI
    {
        NeoneuronRender* _render;
        neon::IdentifiableWrapper<neon::GameObject> _gameObject;
        hey::Listener<std::string> _debugKeyListener;

        DynamicColorPalette _colorPalette;
        DynamicColorPalette _lightColorPalette;

        void initStyle() const;

        void initDebugToggle();

      public:
        NeoneuronUI() = default;

        explicit NeoneuronUI(NeoneuronRender* render);

        ~NeoneuronUI();

        const DynamicColorPalette& getColorPalette() const;

        void setColorPalette(const DynamicColorPalette& palette);

        void setColorPalette(float hue);

        void randomizeColorPalette();

        NeoneuronUI& operator=(NeoneuronUI&& other) noexcept;
    };
} // namespace neoneuron

#endif //NEONEURONUI_H
