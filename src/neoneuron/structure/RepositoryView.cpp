//
// Created by gaeqs on 10/04/2025.
//

#include "RepositoryView.h"

namespace neoneuron
{

    RepositoryView::RepositoryView(Repository* repository, std::vector<GID> neurons, std::vector<GID> synapses) :
        _repository(repository),
        _neurons(std::move(neurons)),
        _synapses(std::move(synapses))
    {
    }

    const std::vector<GID>& RepositoryView::getNeurons() const
    {
        return _neurons;
    }

    const std::vector<GID>& RepositoryView::getSynapses() const
    {
        return _synapses;
    }

    RepositoryView RepositoryView::ofFullRepository(Repository* repository)
    {
        std::vector<GID> neurons;
        std::vector<GID> synapses;

        for (auto [uid, dataset] : repository->getDatasets()) {
            for (mindset::UID neuronUid : dataset->getNeuronsUIDs()) {
                neurons.emplace_back(uid, neuronUid);
            }
            for (mindset::UID synapseUid : dataset->getCircuit().getSynapsesUIDs()) {
                neurons.emplace_back(uid, synapseUid);
            }
        }

        return {repository, std::move(neurons), std::move(synapses)};
    }

} // namespace neoneuron