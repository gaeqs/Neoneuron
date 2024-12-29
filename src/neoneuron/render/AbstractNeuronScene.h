//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSCENE_H
#define ABSTRACTNEURONSCENE_H

#include <rush/rush.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>
#include <neoneuron/render/AbstractSelector.h>

namespace neoneuron {
    class AbstractNeuronScene {
    public:
        virtual ~AbstractNeuronScene() = default;

        [[nodiscard]] virtual AbstractSelector& getSelector() = 0;

        [[nodiscard]] virtual const AbstractSelector& getSelector() const = 0;

        [[nodiscard]] virtual size_t getNeuronsAmount() = 0;

        [[nodiscard]] virtual const std::vector<std::shared_ptr<PrototypeNeuron>>& getPrototypeNeurons() const = 0;

        [[nodiscard]] virtual std::optional<PrototypeNeuron*> findPrototypeNeuron(UID uid) = 0;

        [[nodiscard]] virtual std::optional<const PrototypeNeuron*> findPrototypeNeuron(UID uid) const = 0;

        virtual bool addNeuron(const PrototypeNeuron& neuron) = 0;

        virtual bool addNeuron(PrototypeNeuron&& neuron) = 0;

        virtual bool removeNeuron(UID neuronId) = 0;

        [[nodiscard]] virtual rush::AABB<3, float> getSceneBoundingBox() const = 0;

        virtual void refreshNeuronProperty(UID neuronId, const std::string& propertyName) = 0;
    };
}


#endif //ABSTRACTNEURONSCENE_H
