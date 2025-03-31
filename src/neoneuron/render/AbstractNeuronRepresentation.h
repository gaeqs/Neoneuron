//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSCENE_H
#define ABSTRACTNEURONSCENE_H

#include <rush/rush.h>

#include <mindset/Dataset.h>
#include <mindset/Neuron.h>

#include <neoneuron/render/AbstractSelector.h>

namespace neoneuron
{
    class NeoneuronRender;

    class AbstractNeuronRepresentation
    {
      public:
        virtual ~AbstractNeuronRepresentation() = default;

        [[nodiscard]] virtual NeoneuronRender* getRender() = 0;

        [[nodiscard]] virtual const NeoneuronRender* getRender() const = 0;

        [[nodiscard]] virtual AbstractSelector& getSelector() = 0;

        [[nodiscard]] virtual const AbstractSelector& getSelector() const = 0;

        [[nodiscard]] virtual size_t getNeuronsAmount() = 0;

        [[nodiscard]] virtual size_t getSectionsAmount() = 0;

        [[nodiscard]] virtual size_t getJointsAmount() = 0;

        [[nodiscard]] virtual size_t getSomasAmount() = 0;

        [[nodiscard]] virtual rush::AABB<3, float> getSceneBoundingBox() const = 0;

        virtual void refreshNeuronProperty(mindset::UID neuronId, const std::string& propertyName) = 0;

        [[nodiscard]] virtual mindset::UID findAvailableUID() const = 0;

    };
} // namespace neoneuron

#endif //ABSTRACTNEURONSCENE_H
