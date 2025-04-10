//
// Created by gaeqs on 09/04/2025.
//

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <neoneuron/structure/GID.h>
#include <mindset/mindset.h>

#include <unordered_map>

namespace neoneuron
{

    class Repository
    {
        std::unordered_map<mindset::UID, mindset::Dataset> _datasets;
        mindset::UID _uidGenerator;

      public:
        Repository();

        [[nodiscard]] size_t getDatasetsAmount() const;

        std::tuple<mindset::UID, mindset::Dataset*, bool> addDataset(mindset::Dataset&& dataset);

        bool removeDataset(mindset::UID uid);

        void clear();

        [[nodiscard]] std::optional<mindset::Dataset*> getDataset(mindset::UID uid);

        [[nodiscard]] std::optional<const mindset::Dataset*> getDataset(mindset::UID uid) const;

        [[nodiscard]] std::optional<mindset::Neuron*> getNeuron(GID gid);

        [[nodiscard]] std::optional<const mindset::Neuron*> getNeuron(GID gid) const;

        [[nodiscard]] std::optional<mindset::Synapse*> getSynapse(GID gid);

        [[nodiscard]] std::optional<const mindset::Synapse*> getSynapse(GID gid) const;

        [[nodiscard]] auto getDatasetsUIDs() const
        {
            return _datasets | std::views::keys;
        }

        [[nodiscard]] auto getDatasets()
        {
            return _datasets | std::views::transform([this](auto& pair) { return &pair.second; });
        }

        [[nodiscard]] auto getDatasets() const
        {
            return _datasets | std::views::transform([this](auto& pair) { return &pair.second; });
        }
    };

} // namespace neoneuron

#endif // REPOSITORY_H
