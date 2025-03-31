//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSELECTOR_H
#define ABSTRACTNEURONSELECTOR_H

#include <vector>
#include <any>

namespace neoneuron
{
    struct Selection
    {
        bool clear = false;
        std::vector<std::any> selections = {};
    };

    class AbstractSelector
    {
      public:
        virtual ~AbstractSelector() = default;

        virtual const std::unordered_set<mindset::UID> getSelectedNeurons() = 0;

        virtual void setSelectionData(const Selection& selection) = 0;

        virtual void clearSelection() = 0;

        virtual void selectNeuron(mindset::UID neuronId) = 0;

        virtual void deselectNeuron(mindset::UID neuronId) = 0;

        virtual void selectSection(mindset::UID neuronId, mindset::UID sectionId) = 0;
    };
} // namespace neoneuron

#endif //ABSTRACTNEURONSELECTOR_H
