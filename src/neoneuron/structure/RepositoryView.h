// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef REPOSITORYVIEW_H
#define REPOSITORYVIEW_H

#include <neoneuron/structure/Repository.h>
#include <neoneuron/structure/GID.h>

#include <ranges>
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

        static RepositoryView ofDataset(Repository* repository, NamedDataset* dataset);

        static RepositoryView ofFullRepository(Repository* repository);

        static std::optional<RepositoryView> combine(const std::vector<RepositoryView>& views);
    };

} // namespace neoneuron

#endif // REPOSITORYVIEW_H
