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

#include "LoaderCollection.h"

#include <neon/Neon.h>
#include <neon/util/Chronometer.h>

#include <neoneuron/application/NeoneuronApplication.h>

namespace
{
    void loadThread(const mindset::Loader* loader, neoneuron::NamedDataset* dataset)
    {
        neon::Chronometer chrono;
        loader->load(dataset->getDataset());
        neon::debug() << chrono.elapsedSeconds() << " - Scene loaded. Refreshing scene.";
        neon::debug() << "Time required to load: " << chrono.elapsedSeconds() << " seconds.";
        neon::debug() << "Neurons in dataset: " << dataset->getDataset().getNeurons().size();
        neon::debug() << "Synapses in dataset: " << dataset->getDataset().getCircuit().getSynapses().size();
    }
} // namespace

namespace neoneuron
{
    void LoaderCollection::filterFinishedLoaders() const
    {
        std::erase_if(_loaders, [](const LoaderData& loader) {
            return loader.task->hasFinished() || loader.task->isCancelled();
        });
    }

    LoaderCollection::LoaderCollection(NeoneuronApplication* application) :
        _application(application)
    {
    }

    void LoaderCollection::load(std::unique_ptr<mindset::Loader> loader, std::shared_ptr<NamedDataset> dataset)
    {
        std::shared_ptr<mindset::LoaderStatus> latestStatus = std::make_shared<mindset::LoaderStatus>();
        LoaderData data(std::move(loader), std::move(dataset), nullptr, {}, latestStatus);
        data.statusListener = data.loader->createListener([latestStatus](mindset::LoaderStatus status) {
            *latestStatus = status;
            neon::debug() << status.currentTask << " (" << status.stagesCompleted << "/" << status.stages << ")";
        });

        auto& runner = _application->getRender().getApplication().getTaskRunner();
        data.task = runner.executeAsync(loadThread, data.loader.get(), data.dataset.get());
        _loaders.push_back(std::move(data));
    }

    size_t LoaderCollection::getLoadingAmount() const
    {
        filterFinishedLoaders();
        return _loaders.size();
    }
} // namespace neoneuron