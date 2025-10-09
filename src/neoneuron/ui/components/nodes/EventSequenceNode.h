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

#ifndef EVENTSEQUENCENODE_H
#define EVENTSEQUENCENODE_H

#include "NodeFactory.h"

#include <imblueprint/imblueprint.h>
#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{

    struct EventSequenceId
    {
        mindset::UID datasetId;
        mindset::UID activityId;
        mindset::UID eventSequenceId;

        bool operator==(const EventSequenceId& rhs) const;

        bool operator!=(const EventSequenceId& rhs) const;
    };

    class EventSequenceNode : public ImBlueprint::Node
    {
        NeoneuronApplication* _application;
        int _selectedEventSequence;
        std::optional<EventSequenceId> _selectedEventSequenceId;
        size_t _eventSequenceVersion;
        size_t _activityVersion;

        void sendEventSequence(EventSequenceId id);

      public:
        explicit EventSequenceNode(NeoneuronApplication* application);

        ~EventSequenceNode() override = default;

        void renderBody() override;

        static NodeFactory createFactory();
    };

} // namespace neoneuron

#endif // EVENTSEQUENCENODE_H
