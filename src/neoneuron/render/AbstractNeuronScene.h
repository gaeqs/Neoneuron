//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSCENE_H
#define ABSTRACTNEURONSCENE_H

#include <rush/rush.h>

#include <mnemea/Dataset.h>
#include <mnemea/Neuron.h>

#include <neoneuron/render/AbstractSelector.h>

namespace neoneuron {
    class NeoneuronRender;

    class AbstractNeuronScene {
    public:
        virtual ~AbstractNeuronScene() = default;

        [[nodiscard]] virtual NeoneuronRender* getRender() = 0;

        [[nodiscard]] virtual const NeoneuronRender* getRender() const = 0;

        [[nodiscard]] virtual AbstractSelector& getSelector() = 0;

        [[nodiscard]] virtual const AbstractSelector& getSelector() const = 0;

        [[nodiscard]] virtual size_t getNeuronsAmount() = 0;

        [[nodiscard]] virtual size_t getSectionsAmount() = 0;

        [[nodiscard]] virtual size_t getJointsAmount() = 0;

        [[nodiscard]] virtual size_t getSomasAmount() = 0;

        [[nodiscard]] virtual mnemea::Dataset& getDataset() = 0;

        [[nodiscard]] virtual const mnemea::Dataset& getDataset() const = 0;

        [[nodiscard]] virtual std::optional<mnemea::Neuron*> findPrototypeNeuron(mnemea::UID uid) = 0;

        [[nodiscard]] virtual std::optional<const mnemea::Neuron*> findPrototypeNeuron(mnemea::UID uid) const = 0;

        virtual bool addNeuron(const mnemea::Neuron& neuron) = 0;

        virtual bool addNeuron(mnemea::Neuron&& neuron) = 0;

        virtual bool removeNeuron(mnemea::UID neuronId) = 0;

        [[nodiscard]] virtual rush::AABB<3, float> getSceneBoundingBox() const = 0;

        virtual void refreshNeuronProperty(mnemea::UID neuronId, const std::string& propertyName) = 0;

        [[nodiscard]] virtual mnemea::UID findAvailableUID() const = 0;

        virtual void checkForNewNeurons() = 0;
    };
}


#endif //ABSTRACTNEURONSCENE_H
