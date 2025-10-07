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

#ifndef NEONEURON_TIMEAWAREREPRESENTATION_H
#define NEONEURON_TIMEAWAREREPRESENTATION_H

namespace neoneuron
{

    enum class TimeChangeType
    {
        JUMP,
        FLOW
    };

    /**
     * Represents something that is aware of a timeline.
     */
    class TimeAware
    {
      public:
        virtual ~TimeAware() = default;

        virtual float getStartTime() = 0;

        virtual float getEndTime() = 0;

        virtual void onTimeChanged(float lastTime, float newTime, TimeChangeType type) = 0;
    };

} // namespace neoneuron

#endif // NEONEURON_TIMEAWAREREPRESENTATION_H
