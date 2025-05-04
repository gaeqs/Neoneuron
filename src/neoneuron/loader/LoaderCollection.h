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

#ifndef LOADERCOLLECTION_H
#define LOADERCOLLECTION_H

#include <neoneuron/loader/LoaderData.h>
#include <neoneuron/structure/NamedDataset.h>

namespace neoneuron
{

    class NeoneuronApplication;

    /**
     * Collection of running loaders.
     */
    class LoaderCollection
    {
        NeoneuronApplication* _application;

        mutable std::vector<LoaderData> _loaders;

        void filterFinishedLoaders() const;

      public:
        explicit LoaderCollection(NeoneuronApplication* application);

        void load(std::unique_ptr<mindset::Loader> loader, std::shared_ptr<NamedDataset> dataset);

        size_t getLoadingAmount() const;
    };
} // namespace neoneuron

#endif // LOADERCOLLECTION_H
