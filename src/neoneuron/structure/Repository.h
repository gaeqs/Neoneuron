//
// Created by gaeqs on 09/04/2025.
//

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <ranges>
#include <optional>

#include <neoneuron/structure/GID.h>
#include <mindset/mindset.h>

#include <unordered_map>

namespace neoneuron
{

    class Repository : public mindset::Versioned
    {
        std::unordered_map<mindset::UID, mindset::Dataset> _datasets;
        mindset::UID _uidGenerator;

        hey::Listener<mindset::Neuron*> _neuronAddedListener;
        hey::Listener<mindset::UID> _neuronRemovedListener;
        hey::Listener<void*> _clearListener;

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

        [[nodiscard]] std::optional<std::pair<mindset::Dataset*, mindset::Neuron*>> getNeuronAndDataset(GID gid);

        [[nodiscard]] std::optional<std::pair<const mindset::Dataset*, const mindset::Neuron*>> getNeuronAndDataset(
            GID gid) const;

        [[nodiscard]] std::optional<mindset::Synapse*> getSynapse(GID gid);

        [[nodiscard]] std::optional<const mindset::Synapse*> getSynapse(GID gid) const;

        [[nodiscard]] auto getDatasetsUIDs() const
        {
            return _datasets | std::views::keys;
        }

        [[nodiscard]] decltype(auto) getDatasets()
        {
            return _datasets |
                   std::views::transform([this](auto& pair) { return std::make_pair(pair.first, &pair.second); });
        }

        [[nodiscard]] decltype(auto) getDatasets() const
        {
            return _datasets |
                   std::views::transform([this](auto& pair) { return std::make_pair(pair.first, &pair.second); });
        }

        [[nodiscard]] decltype(auto) getNeurons()
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getNonContextualizedNeurons() |
                              std::views::transform([uid = pair.first](auto* neuron) {
                                  return std::make_pair(GID(uid, neuron->getUID()), neuron);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getNeurons() const
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getNonContextualizedNeurons() |
                              std::views::transform([uid = pair.first](const auto* neuron) {
                                  return std::make_pair(GID(uid, neuron->getUID()), neuron);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getNeuronsWithDatasets()
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getNonContextualizedNeurons() |
                              std::views::transform(
                                  [dataset = pair.second](auto* neuron) { return std::make_pair(dataset, neuron); });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getNeuronsWithDatasets() const
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getNonContextualizedNeurons() |
                              std::views::transform([dataset = pair.second](const auto* neuron) {
                                  return std::make_pair(dataset, neuron);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getSynapses()
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getCircuit().getSynapses() |
                              std::views::transform([uid = pair.first](auto* synapse) {
                                  return std::make_pair(GID(uid, synapse->getUID()), synapse);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getSynapses() const
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getCircuit().getSynapses() |
                              std::views::transform([uid = pair.first](auto* synapse) {
                                  return std::make_pair(GID(uid, synapse->getUID()), synapse);
                              });
                   }) |
                   std::views::join;
        }
    };

} // namespace neoneuron

#endif // REPOSITORY_H
