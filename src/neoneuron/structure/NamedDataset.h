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

#ifndef NEONEURON_NAMEDDATASET_H
#define NEONEURON_NAMEDDATASET_H

#include <mindset/mindset.h>

namespace neoneuron
{
    class NamedDataset
    {
        mindset::UID _uid;
        std::string _name;
        mindset::Dataset _dataset;

      public:
        NamedDataset(mindset::UID uid, std::string name, mindset::Dataset dataset);

        [[nodiscard]] mindset::UID getUID() const;

        [[nodiscard]] const std::string& getName() const;

        void setName(const std::string& name);

        [[nodiscard]] mindset::Dataset& getDataset();

        [[nodiscard]] const mindset::Dataset& getDataset() const;
    };
} // namespace neoneuron

#endif // NEONEURON_NAMEDDATASET_H
