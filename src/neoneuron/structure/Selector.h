//
// Created by gaeqs on 31/03/25.
//

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

    class Selector
    {
        const Repository* _repository;

        std::unordered_set<SelectionEntry> _selectedSegments;
        std::unordered_set<GID> _selectedNeurons;
        std::unordered_set<GID> _selectedSynapses;

        hey::Observable<SelectionEvent> _selectionEvent;

        void clear();

      public:
        explicit Selector(const Repository* dataset);

        const std::unordered_set<SelectionEntry>& getSelectedSections() const;

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
