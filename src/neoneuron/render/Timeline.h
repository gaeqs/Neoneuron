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
// Created by gaeqs on 6/10/25.
//

#ifndef NEONEURON_TIMELINE_H
#define NEONEURON_TIMELINE_H

#include <unordered_set>
#include <neon/structure/Component.h>
#include <neoneuron/render/TimeAware.h>

namespace neoneuron
{
    class Timeline : public neon::Component
    {
        std::unordered_set<std::shared_ptr<TimeAware>> _timeAwares;

        float _minTime;
        float _maxTime;

        void recalculateTimelineSize();

      public:
        Timeline();

        void addTimeAware(std::shared_ptr<TimeAware> timeAware);

        void removeTimeAware(std::shared_ptr<TimeAware> timeAware);

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // NEONEURON_TIMELINE_H
