//
// Created by gaeqs on 10/04/2025.
//

#ifndef REPOSITORYVIEW_H
#define REPOSITORYVIEW_H

#include <neoneuron/structure/Repository.h>
#include <neoneuron/structure/GID.h>

#include <vector>

namespace neoneuron
{

    class RepositoryView
    {
        Repository* _repository;

        std::vector<GID> _neurons;
        std::vector<GID> _synapses;

      public:
        RepositoryView(Repository* repository, std::vector<GID> neurons, std::vector<GID> synapses);

        [[nodiscard]] Repository* getRepository() const;

        [[nodiscard]] const std::vector<GID>& getNeurons() const;

        [[nodiscard]] const std::vector<GID>& getSynapses() const;

        decltype(auto) getNeurons()
        {
            return _neurons |
                   std::views::transform([rep = _repository](const GID& gid) { return rep->getNeuron(gid); }) |
                   std::views::filter([](const auto& opt) { return opt.has_value(); }) |
                   std::views::transform([](const auto& opt) { return opt.value(); });
        }

        decltype(auto) getSynapses()
        {
            return _synapses |
                   std::views::transform([rep = _repository](const GID& gid) { return rep->getSynapse(gid); }) |
                   std::views::filter([](const auto& opt) { return opt.has_value(); }) |
                   std::views::transform([](const auto& opt) { return opt.value(); });
        }


        static RepositoryView ofFullRepository(Repository* repository);
    };

} // namespace neoneuron

#endif // REPOSITORYVIEW_H
