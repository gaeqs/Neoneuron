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

#include "Selector.h"

namespace neoneuron
{

    void Selector::clear()
    {
        _selectedSegments.clear();
        _selectedNeurons.clear();
        _selectedSynapses.clear();
    }

    Selector::Selector(const Repository* dataset) :
        _repository(dataset)
    {
    }

    const std::unordered_set<SelectionEntry>& Selector::getSelectedSegments() const
    {
        return _selectedSegments;
    }

    const std::unordered_set<GID>& Selector::getSelectedNeurons() const
    {
        return _selectedNeurons;
    }

    const std::unordered_set<GID>& Selector::getSelectedSynapses() const
    {
        return _selectedSynapses;
    }

    void Selector::selectNeuron(SelectionMode selectionMode, GID neuronId)
    {
        SelectionEvent event{.selector = this, .segmentsChanged = true, .synapsesChanged = false};
        if (selectionMode == SelectionMode::OVERRIDE_ALL) {
            event.synapsesChanged = !_selectedSynapses.empty();
            clear();
        } else if (selectionMode == SelectionMode::OVERRIDE_TYPE) {
            _selectedSegments.clear();
            _selectedNeurons.clear();
        }

        _selectedNeurons.insert(neuronId);

        auto neuron = _repository->getNeuron(neuronId);
        if (neuron.has_value()) {
            if (auto morphology = neuron.value()->getMorphology()) {
                for (auto uid : morphology.value()->getNeuritesUIDs()) {
                    _selectedSegments.insert({neuronId, uid});
                }
                if (auto soma = morphology.value()->getSoma()) {
                    _selectedSegments.insert({neuronId, soma.value()->getUID()});
                }
            }
        }

        _selectionEvent.invoke(event);
        incrementVersion();
    }

    void Selector::selectSegment(SelectionMode selectionMode, GID neuronId, mindset::UID segmentId)
    {
        SelectionEvent event{.selector = this, .segmentsChanged = true, .synapsesChanged = false};
        if (selectionMode == SelectionMode::OVERRIDE_ALL) {
            event.synapsesChanged = !_selectedSynapses.empty();
            clear();
        } else if (selectionMode == SelectionMode::OVERRIDE_TYPE) {
            _selectedSegments.clear();
            _selectedNeurons.clear();
        }

        _selectedNeurons.insert(neuronId);
        _selectedSegments.insert({neuronId, segmentId});
        _selectionEvent.invoke(event);
        incrementVersion();
    }

    void Selector::selectSegments(SelectionMode selectionMode, const std::vector<std::pair<GID, mindset::UID>>& pairs)
    {
        SelectionEvent event{.selector = this, .segmentsChanged = true, .synapsesChanged = false};
        if (selectionMode == SelectionMode::OVERRIDE_ALL) {
            event.synapsesChanged = !_selectedSynapses.empty();
            clear();
        } else if (selectionMode == SelectionMode::OVERRIDE_TYPE) {
            _selectedSegments.clear();
            _selectedNeurons.clear();
        }

        for (auto [neuronId, segmentId] : pairs) {
            _selectedNeurons.insert(neuronId);
            _selectedSegments.insert({neuronId, segmentId});
        }

        _selectionEvent.invoke(event);
        incrementVersion();
    }

    void Selector::selectSynapse(SelectionMode selectionMode, GID synapseId)
    {
        SelectionEvent event{.selector = this, .segmentsChanged = false, .synapsesChanged = true};
        if (selectionMode == SelectionMode::OVERRIDE_ALL) {
            event.segmentsChanged = !_selectedSegments.empty();
            clear();
        } else if (selectionMode == SelectionMode::OVERRIDE_TYPE) {
            _selectedSynapses.clear();
        }

        _selectedSynapses.insert(synapseId);
        _selectionEvent.invoke(event);
        incrementVersion();
    }

    void Selector::deselectNeuron(GID neuronId)
    {
        bool result = _selectedNeurons.erase(neuronId);
        if (!result) {
            return;
        }

        std::erase_if(_selectedSegments, [neuronId](const SelectionEntry& entry) { return entry.neuron == neuronId; });
        _selectionEvent.invoke({this, true, false});
        incrementVersion();
    }

    void Selector::clearSelection()
    {
        SelectionEvent event{.selector = this,
                             .segmentsChanged = !_selectedSegments.empty(),
                             .synapsesChanged = !_selectedSynapses.empty()};
        clear();
        _selectionEvent.invoke(event);
        incrementVersion();
    }

    hey::Observable<SelectionEvent>& Selector::onSelectionEvent()
    {
        return _selectionEvent;
    }

    bool SelectionEntry::operator==(const SelectionEntry& other) const
    {
        return neuron == other.neuron && segment == other.segment;
    }

    bool SelectionEntry::operator!=(const SelectionEntry& other) const
    {
        return neuron != other.neuron || segment != other.segment;
    }
} // namespace neoneuron