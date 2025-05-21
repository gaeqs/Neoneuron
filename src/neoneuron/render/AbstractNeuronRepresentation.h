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

#ifndef ABSTRACTNEURONSCENE_H
#define ABSTRACTNEURONSCENE_H

#include <unordered_set>

#include <rush/rush.h>
#include <neoneuron/structure/GID.h>
#include <neoneuron/structure/RepositoryView.h>
#include <neoneuron/render/Viewport.h>

namespace neoneuron
{
    class NeoneuronRender;

    class AbstractNeuronRepresentation
    {
      public:
        virtual ~AbstractNeuronRepresentation() = default;

        [[nodiscard]] virtual NeoneuronRender* getRender() = 0;

        [[nodiscard]] virtual const NeoneuronRender* getRender() const = 0;

        [[nodiscard]] virtual rush::AABB<3, float> getSceneBoundingBox() const = 0;

        virtual void refreshNeuronProperty(GID neuronId, const std::string& propertyName) = 0;

        virtual void refreshData(const RepositoryView& view) = 0;

        virtual void clearData() = 0;

        virtual void addViewport(Viewport* viewport) = 0;

        virtual void setViewports(const std::unordered_set<Viewport*>& viewport) = 0;

        virtual void removeViewport(Viewport* viewport) = 0;

        [[nodiscard]] virtual size_t getTotalAllocatedMemory() const = 0;

        [[nodiscard]] virtual size_t getAllocatedInstanceMemory() const = 0;

        [[nodiscard]] virtual size_t getUsedInstanceMemory() const = 0;

        [[nodiscard]] virtual float getUsedInstanceMemoryPercentage() const = 0;
    };
} // namespace neoneuron

#endif // ABSTRACTNEURONSCENE_H
