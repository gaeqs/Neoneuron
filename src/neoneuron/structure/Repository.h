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

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <ranges>
#include <optional>

#include <mindset/mindset.h>

#include <unordered_map>

#include <neoneuron/structure/GID.h>
#include <neoneuron/structure/NamedDataset.h>

namespace neoneuron
{

    class Repository : public mindset::Versioned
    {
        std::unordered_map<mindset::UID, std::shared_ptr<NamedDataset>> _datasets;
        mindset::UID _uidGenerator;

        hey::Listener<mindset::Neuron*> _neuronAddedListener;
        hey::Listener<mindset::Synapse*> _synapseAddedListener;
        hey::Listener<mindset::UID> _elementRemovedListener;
        hey::Listener<void*> _clearListener;

      public:
        Repository();

        [[nodiscard]] size_t getDatasetsAmount() const;

        std::tuple<mindset::UID, NamedDataset*, bool> addDataset(mindset::Dataset&& dataset, std::string name);

        bool removeDataset(mindset::UID uid);

        void clear();

        [[nodiscard]] std::optional<std::shared_ptr<NamedDataset>> getDatasetAsOwned(mindset::UID uid);

        [[nodiscard]] std::optional<NamedDataset*> getDataset(mindset::UID uid);

        [[nodiscard]] std::optional<const NamedDataset*> getDataset(mindset::UID uid) const;

        [[nodiscard]] std::optional<mindset::Neuron*> getNeuron(GID gid);

        [[nodiscard]] std::optional<const mindset::Neuron*> getNeuron(GID gid) const;

        [[nodiscard]] std::optional<std::pair<NamedDataset*, mindset::Neuron*>> getNeuronAndDataset(GID gid);

        [[nodiscard]] std::optional<std::pair<const NamedDataset*, const mindset::Neuron*>> getNeuronAndDataset(
            GID gid) const;

        [[nodiscard]] std::optional<mindset::Synapse*> getSynapse(GID gid);

        [[nodiscard]] std::optional<const mindset::Synapse*> getSynapse(GID gid) const;

        [[nodiscard]] std::optional<std::pair<NamedDataset*, mindset::Synapse*>> getSynapseAndDataset(GID gid);

        [[nodiscard]] std::optional<std::pair<const NamedDataset*, const mindset::Synapse*>> getSynapseAndDataset(
            GID gid) const;

        [[nodiscard]] auto getDatasetsUIDs() const
        {
            return _datasets | std::views::keys;
        }

        [[nodiscard]] decltype(auto) getDatasets()
        {
            return _datasets |
                   std::views::transform([this](auto& pair) { return std::make_pair(pair.first, pair.second.get()); });
        }

        [[nodiscard]] decltype(auto) getDatasets() const
        {
            return _datasets |
                   std::views::transform([this](auto& pair) { return std::make_pair(pair.first, pair.second.get()); });
        }

        [[nodiscard]] decltype(auto) getNeurons()
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getDataset().getNonContextualizedNeurons() |
                              std::views::transform([uid = pair.first](auto* neuron) {
                                  return std::make_pair(GID(uid, neuron->getUID()), neuron);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getNeurons() const
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getDataset().getNonContextualizedNeurons() |
                              std::views::transform([uid = pair.first](const auto* neuron) {
                                  return std::make_pair(GID(uid, neuron->getUID()), neuron);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getNeuronsWithDatasets()
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getDataset().getNonContextualizedNeurons() |
                              std::views::transform(
                                  [dataset = pair.second](auto* neuron) { return std::make_pair(dataset, neuron); });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getNeuronsWithDatasets() const
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getDataset().getNonContextualizedNeurons() |
                              std::views::transform([dataset = pair.second](const auto* neuron) {
                                  return std::make_pair(dataset, neuron);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getSynapses()
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getDataset().getCircuit().getSynapses() |
                              std::views::transform([uid = pair.first](auto* synapse) {
                                  return std::make_pair(GID(uid, synapse->getUID()), synapse);
                              });
                   }) |
                   std::views::join;
        }

        [[nodiscard]] decltype(auto) getSynapses() const
        {
            return getDatasets() | std::views::transform([](const auto& pair) {
                       return pair.second->getDataset().getCircuit().getSynapses() |
                              std::views::transform([uid = pair.first](auto* synapse) {
                                  return std::make_pair(GID(uid, synapse->getUID()), synapse);
                              });
                   }) |
                   std::views::join;
        }
    };

} // namespace neoneuron

#endif // REPOSITORY_H
