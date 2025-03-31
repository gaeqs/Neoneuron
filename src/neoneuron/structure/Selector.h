//
// Created by gaeqs on 31/03/25.
//

#ifndef SELECTOR_H
#define SELECTOR_H

#include <mindset/mindset.h>
#include <hey/Hey.h>

#include <unordered_set>

namespace neoneuron
{
    struct SelectionEntry
    {
        mindset::UID neuron;
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
        size_t hash1 = hasher(entry.neuron);
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

    class Selector
    {
        const mindset::Dataset* _dataset;

        std::unordered_set<SelectionEntry> _selectedSegments;
        std::unordered_set<mindset::UID> _selectedNeurons;
        std::unordered_set<mindset::UID> _selectedSynapses;

        hey::Observable<SelectionEvent> _selectionEvent;

        void clear();

      public:
        explicit Selector(const mindset::Dataset* dataset);

        const std::unordered_set<SelectionEntry>& getSelectedSections() const;

        const std::unordered_set<mindset::UID>& getSelectedNeurons() const;

        const std::unordered_set<mindset::UID>& getSelectedSynapses() const;

        void selectNeuron(SelectionMode selectionMode, mindset::UID neuronId);

        void selectSegment(SelectionMode selectionMode, mindset::UID neuronId, mindset::UID segmentId);

        void selectSegments(SelectionMode selectionMode, std::vector<std::pair<mindset::UID, mindset::UID>> pairs);

        void selectSynapse(SelectionMode selectionMode, mindset::UID synapseId);

        void deselectNeuron(mindset::UID neuronId);

        void clearSelection();

        hey::Observable<SelectionEvent>& onSelectionEvent();
    };

} // namespace neoneuron

#endif //SELECTOR_H
