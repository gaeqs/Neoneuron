//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSCENE_H
#define ABSTRACTNEURONSCENE_H


#include <rush/rush.h>

#include <mindset/Dataset.h>
#include <mindset/Neuron.h>

namespace neoneuron
{
    class NeoneuronRender;

    class AbstractNeuronRepresentation
    {
      public:
        virtual ~AbstractNeuronRepresentation() = default;

        [[nodiscard]] virtual NeoneuronRender* getRender() = 0;

        [[nodiscard]] virtual const NeoneuronRender* getRender() const = 0;

        [[nodiscard]] virtual rush::AABB<3, float> getSceneBoundingBox() const = 0;

        virtual void refreshNeuronProperty(mindset::UID neuronId, const std::string& propertyName) = 0;

    };
} // namespace neoneuron

#endif //ABSTRACTNEURONSCENE_H
