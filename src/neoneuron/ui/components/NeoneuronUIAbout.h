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
// Created by gaeqs on 6/05/25.
//

#ifndef NEONEURONUIABOUT_H
#define NEONEURONUIABOUT_H

#include <neon/Neon.h>

namespace neoneuron
{
    class NeoneuronApplication;

    class NeoneuronUIAbout : public neon::Component
    {
        static constexpr float SIDEBAR_SIZE = 128.0f;
        static constexpr float ICON_SIZE = 96.0f;

        struct IconEntry
        {
            std::shared_ptr<neon::SampledTexture> icon;
            VkDescriptorSet descriptor;
            ImTextureID textureId;
        };

        std::vector<IconEntry> _icons;

        uint32_t _vulkanAPIVersion;
        std::string _glfwVersion;
        std::string _license;

        bool _open;

        std::string getPlatform() const;

        std::string getOSName() const;

        std::string getLibCName() const;

        void showIcon() const;

        void showTitle() const;

        void showVersionInfo() const;

        void showAcknowledges() const;

        void showLibraries() const;

        void showEnvironment() const;

        void showLicense() const;

        void loadIcons(NeoneuronApplication* application);

      public:
        NeoneuronUIAbout(NeoneuronApplication* application);

        ~NeoneuronUIAbout() override;

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // NEONEURONUIABOUT_H
