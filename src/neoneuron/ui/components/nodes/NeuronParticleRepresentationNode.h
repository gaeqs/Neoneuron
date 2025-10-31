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

#ifndef NEONEURON_ACTIVITYREPRESENTATIONNODE_H
#define NEONEURON_ACTIVITYREPRESENTATIONNODE_H

#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/particle/ParticleNeuronRepresentation.h>
#include <neoneuron/ui/components/nodes/NeoneuronNode.h>
#include <neoneuron/ui/components/nodes/NodeFactory.h>

namespace neoneuron
{

    class NeuronParticleRepresentationNode : public NeoneuronNode
    {
        NeoneuronApplication* _application;
        std::weak_ptr<ParticleNeuronRepresentation> _representation;

        void drawProgressBar(ParticleNeuronRepresentation* ptr);

      public:
        explicit NeuronParticleRepresentationNode(NeoneuronApplication* application);

        ~NeuronParticleRepresentationNode() override;

        void renderBody() override;

        void onInputChange(const std::string& name, const std::any& value) override;

        static NodeFactory createFactory();
    };

} // namespace neoneuron

#endif // NEONEURON_ACTIVITYREPRESENTATIONNODE_H
