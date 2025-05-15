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

//
// Created by gaeqs on 15/05/25.
//

#ifndef NEONEURON_TOOLWINDOW_H
#define NEONEURON_TOOLWINDOW_H

#include <string>
#include <neon/structure/Component.h>
#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{
    class ToolWindow : public neon::Component
    {
        NeoneuronApplication* _application;
        std::string _configNode;
        std::string _windowName;

      protected:
        void setWindowName(std::string windowName);

        virtual void drawWindowContents() = 0;

      public:
        ToolWindow(NeoneuronApplication* application, std::string configNode, std::string windowName);

        NeoneuronApplication* getNeoneuronApplication() const;

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // NEONEURON_TOOLWINDOW_H
