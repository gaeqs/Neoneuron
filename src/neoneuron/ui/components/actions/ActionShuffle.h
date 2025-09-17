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

#ifndef ACTIONSHUFFLE_H
#define ACTIONSHUFFLE_H
#include <imgui.h>
#include <random>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/components/ModalComponent.h>

namespace neoneuron
{
    class ActionShuffle : public ModalComponent
    {
        NeoneuronApplication* _application;
        rush::Vec3f _center;
        float _radius;
        bool _shuffleRotation;

        std::random_device _randomDevice;
        std::mt19937 _randomGenerator;

        void shuffle(GID gid, NamedDataset* dataset, mindset::Neuron* neuron);

        void run();

      public:
        explicit ActionShuffle(NeoneuronApplication* application);

        void onModalDraw() override;

        void actionButton(ImGui::neon::LinearLayout& layout) override;
    };
} // namespace neoneuron

#endif //ACTIONSHUFFLE_H
