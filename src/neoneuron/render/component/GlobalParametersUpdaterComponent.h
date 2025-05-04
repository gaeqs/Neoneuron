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

#ifndef NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H
#define NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H

#include <array>
#include <neon/Neon.h>
#include <neoneuron/render/Uniforms.h>

namespace neoneuron
{
    class NeoneuronRender;

    class GlobalParametersUpdaterComponent : public neon::Component
    {
        NeoneuronRender& _render;

      public:
        explicit GlobalParametersUpdaterComponent(NeoneuronRender& render);

        ~GlobalParametersUpdaterComponent() override;

        void onStart() override;

        void onPreDraw() override;
    };

    REGISTER_COMPONENT(GlobalParametersUpdaterComponent, "Global Parameters Updater")
} // namespace neoneuron

#endif //NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H
