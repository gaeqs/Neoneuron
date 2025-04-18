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

    Repository* RepositoryView::getRepository() const
    {
        return _repository;
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

    std::optional<RepositoryView> RepositoryView::combine(const std::vector<RepositoryView>& views)
    {
        if (views.empty()) {
            return {};
        }
        if (views.size() == 1) {
            return views[0];
        }

        Repository* repository = nullptr;
        std::unordered_set<GID> neurons;
        std::unordered_set<GID> synapses;

        for (auto& view : views) {
            if (repository == nullptr && view.getRepository() != nullptr) {
                repository = view.getRepository();
            }
            neurons.insert(view.getNeurons().begin(), view.getNeurons().end());
            synapses.insert(view.getSynapses().begin(), view.getSynapses().end());
        }

        return RepositoryView(repository, std::vector(neurons.begin(), neurons.end()),
                              std::vector(synapses.begin(), synapses.end()));
    }

} // namespace neoneuron