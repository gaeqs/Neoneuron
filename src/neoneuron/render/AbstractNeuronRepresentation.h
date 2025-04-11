//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSCENE_H
#define ABSTRACTNEURONSCENE_H

#include <rush/rush.h>
#include <neoneuron/structure/GID.h>
#include <neoneuron/structure/RepositoryView.h>

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

        virtual void refreshNeuronProperty(GID neuronId, const std::string& propertyName) = 0;

        virtual void refreshData(const RepositoryView& view) = 0;

        virtual void clearData() = 0;
    };
} // namespace neoneuron

#endif // ABSTRACTNEURONSCENE_H
