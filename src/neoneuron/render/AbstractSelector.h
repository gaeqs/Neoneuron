//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSELECTOR_H
#define ABSTRACTNEURONSELECTOR_H

#include <vector>
#include <any>

namespace neoneuron {
    struct Selection {
        bool clear = false;
        std::vector<std::any> selections = {};
    };

    class AbstractSelector {
    public:
        virtual ~AbstractSelector() = default;

        virtual const std::unordered_set<UID> getSelectedNeurons() = 0;

        virtual void setSelectionData(const Selection& selection) = 0;

        virtual void clearSelection() = 0;

        virtual void selectNeuron(UID neuronId) = 0;

        virtual void deselectNeuron(UID neuronId) = 0;

        virtual void selectSection(UID neuronId, UID sectionId) = 0;
    };
}

#endif //ABSTRACTNEURONSELECTOR_H
