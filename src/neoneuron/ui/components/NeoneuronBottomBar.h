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

#ifndef NEONEURONBOTTOMBAR_H
#define NEONEURONBOTTOMBAR_H

#include "../../../../cmake-build-release/_deps/neon-src/src/neon/util/component/DockSpaceComponent.h"

#include <neon/Neon.h>

namespace neoneuron
{
    class NeoneuronApplication;

    class NeoneuronBottomBar : public neon::Component
    {

        static constexpr float LOADING_STATUS_WIDTH = 300.0f;

        NeoneuronApplication* _application;
        neon::IdentifiableWrapper<neon::DockSpaceComponent> _dockSpace;
        neon::DockSidebar* _sidebar;

        void multiLoaderStatusBar() const;

        void singleLoaderStatusBar() const;

        void loadingStatus() const;

      public:
        NeoneuronBottomBar(NeoneuronApplication* application,
                           neon::IdentifiableWrapper<neon::DockSpaceComponent> dockSpace);

        ~NeoneuronBottomBar() override;

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // NEONEURONBOTTOMBAR_H
