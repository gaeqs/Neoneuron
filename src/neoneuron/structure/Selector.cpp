//
// Created by gaeqs on 31/03/25.
//

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

    const std::unordered_set<SelectionEntry>& Selector::getSelectedSections() const
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

        //if (auto morphology = getMorphology(*_repository, neuronId)) {
        //    for (auto uid : morphology.value()->getNeuritesUIDs()) {
        //        _selectedSegments.insert({neuronId, uid});
        //    }
        //}

        _selectionEvent.invoke(event);
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
    }
    void Selector::deselectNeuron(GID neuronId)
    {
        bool result = _selectedNeurons.erase(neuronId);
        if (!result) {
            return;
        }

        std::erase_if(_selectedSegments, [neuronId](const SelectionEntry& entry) { return entry.neuron == neuronId; });
        _selectionEvent.invoke({this, true, false});
    }

    void Selector::clearSelection()
    {
        SelectionEvent event{.selector = this,
                             .segmentsChanged = !_selectedSegments.empty(),
                             .synapsesChanged = !_selectedSynapses.empty()};
        clear();
        _selectionEvent.invoke(event);
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