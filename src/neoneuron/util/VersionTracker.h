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
// Created by gaeqs on 7/10/25.
//

#ifndef NEONEURON_VERSIONTRACKER_H
#define NEONEURON_VERSIONTRACKER_H

#include <mindset/Versioned.h>

namespace neoneuron
{

    template<typename T>
    concept IsVersioned = std::is_base_of_v<mindset::Versioned, T>;

    template<IsVersioned T>
    class VersionTracker
    {
        T* _trackedObject;
        uint64_t _lastKnownVersion;

      public:
        explicit VersionTracker(T* tracked) :
            _trackedObject(tracked),
            _lastKnownVersion(tracked->getVersion())
        {
        }

        bool hasChanged()
        {
            if (auto currentVersion = _trackedObject->getVersion(); currentVersion != _lastKnownVersion) {
                _lastKnownVersion = currentVersion;
                return true;
            }
            return false;
        }

        T* getTrackedObject()
        {
            return _trackedObject;
        }

        const T* getTrackedObject() const
        {
            return _trackedObject;
        }

        [[nodiscard]] uint64_t getLastKnownVersion() const
        {
            return _lastKnownVersion;
        }
    };

} // namespace neoneuron

#endif // NEONEURON_VERSIONTRACKER_H
