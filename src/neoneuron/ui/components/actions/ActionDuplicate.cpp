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
// Created by gaeqs on 17/06/25.
//

#include "ActionDuplicate.h"

namespace neoneuron
{

    void ActionDuplicate::run() const
    {
        for (auto [did, dataset] : _application->getRepository().getDatasets()) {
            auto& ds = dataset->getDataset();
            auto lock = ds.writeLock();

            std::vector<mindset::Neuron> neurons;
            for (auto neuron : ds.getNonContextualizedNeurons()) {
                neurons.push_back(*neuron);
            }

            for (int i = 0; i < _times - 1; ++i) {
                for (auto& neuron : neurons) {
                    neuron.setUID(ds.findSmallestAvailableNeuronUID());
                    ds.addNeuron(neuron);
                }
            }
            if (_times > 0) {
                for (auto& neuron : neurons) {
                    neuron.setUID(ds.findSmallestAvailableNeuronUID());
                    ds.addNeuron(std::move(neuron));
                }
            }
        }
    }

    ActionDuplicate::ActionDuplicate(NeoneuronApplication* application) :
        ModalComponent("Duplicate scene", true),
        _application(application),
        _times(1)
    {
    }

    void ActionDuplicate::onModalDraw()
    {
        if (ImGui::InputInt("Times", &_times, 1, 1)) {
            _times = std::clamp(_times, 1, 10);
        }
    }

    void ActionDuplicate::actionButton(ImGui::neon::LinearLayout& layout)
    {
        if (layout.button("Run", ImVec2(120.0f, 0.0f))) {
            run();
            ImGui::CloseCurrentPopup();
        }
    }
} // namespace neoneuron