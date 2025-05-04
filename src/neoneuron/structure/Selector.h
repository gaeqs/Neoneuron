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

#ifndef SELECTOR_H
#define SELECTOR_H

#include <mindset/mindset.h>
#include <neoneuron/structure/Repository.h>

#include <unordered_set>

namespace neoneuron
{
    struct SelectionEntry
    {
        GID neuron;
        mindset::UID segment;

        bool operator==(const SelectionEntry& other) const;

        bool operator!=(const SelectionEntry& other) const;
    };
} // namespace neoneuron

template<>
struct std::hash<neoneuron::SelectionEntry>
{
    size_t operator()(const neoneuron::SelectionEntry& entry) const noexcept
    {
        std::hash<uint32_t> hasher;
        std::hash<neoneuron::GID> gidHasher;
        size_t hash1 = gidHasher(entry.neuron);
        size_t hash2 = hasher(entry.segment);
        return hash1 ^ hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
    }
};

namespace neoneuron
{

    class Selector;

    enum class SelectionMode
    {
        APPEND,
        OVERRIDE_TYPE,
        OVERRIDE_ALL
    };

    struct SelectionEvent
    {
        const Selector* selector;
        bool segmentsChanged;
        bool synapsesChanged;
    };

    class Selector : public mindset::Versioned
    {
        const Repository* _repository;

        std::unordered_set<SelectionEntry> _selectedSegments;
        std::unordered_set<GID> _selectedNeurons;
        std::unordered_set<GID> _selectedSynapses;

        hey::Observable<SelectionEvent> _selectionEvent;

        void clear();

      public:
        explicit Selector(const Repository* dataset);

        const std::unordered_set<SelectionEntry>& getSelectedSegments() const;

        const std::unordered_set<GID>& getSelectedNeurons() const;

        const std::unordered_set<GID>& getSelectedSynapses() const;

        void selectNeuron(SelectionMode selectionMode, GID neuronId);

        void selectSegment(SelectionMode selectionMode, GID neuronId, mindset::UID segmentId);

        void selectSegments(SelectionMode selectionMode, const std::vector<std::pair<GID, mindset::UID>>& pairs);

        void selectSynapse(SelectionMode selectionMode, GID synapseId);

        void deselectNeuron(GID neuronId);

        void clearSelection();

        hey::Observable<SelectionEvent>& onSelectionEvent();
    };

} // namespace neoneuron

#endif // SELECTOR_H
