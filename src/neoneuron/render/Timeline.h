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
#include <neoneuron/structure/GID.h>
#include <neoneuron/render/TimeAware.h>

namespace neoneuron
{

    class NeoneuronApplication;

    struct SampledEntry
    {
        uint64_t version = -1;
        bool deleteFlag = false;

        std::chrono::nanoseconds duration;

        std::vector<double> times;
        std::vector<double> samples;
    };

    struct SampledActivity
    {
        uint64_t version = -1;
        bool deleteFlag = false;
        std::map<std::string, SampledEntry> entries;
    };

    class Timeline : public neon::Component
    {
        NeoneuronApplication* _application;
        std::unordered_set<std::shared_ptr<TimeAware>> _timeAwares;
        std::unordered_map<GID, SampledActivity> _samples;

        std::chrono::nanoseconds _duration;

        bool _fitNextFrame;

        static void sample(SampledEntry& entry, const mindset::EventSequence<std::monostate>* sampler,
                           std::chrono::nanoseconds step);

        static void sample(SampledEntry& entry, const mindset::TimeGrid<double>* sampler);

        void checkTimelines();

        void recalculateTimelineSize();

      public:
        Timeline(NeoneuronApplication* application);

        void addTimeAware(std::shared_ptr<TimeAware> timeAware);

        void removeTimeAware(std::shared_ptr<TimeAware> timeAware);

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // NEONEURON_TIMELINE_H
