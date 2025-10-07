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

#ifndef NEONEURON_TIMELINENODE_H
#define NEONEURON_TIMELINENODE_H

#include <imblueprint/imblueprint.h>

#include "NodeFactory.h"
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>

namespace neoneuron
{
    class TimelineNode : public ImBlueprint::Node
    {
        NeoneuronApplication* _application;
        Timeline* _timeline;

      public:
        explicit TimelineNode(NeoneuronApplication* application);

        ~TimelineNode() override;

        void onInputChange(const std::string& name, const std::any& value) override;

        static NodeFactory createFactory();
    };
} // namespace neoneuron

#endif // NEONEURON_TIMELINENODE_H
