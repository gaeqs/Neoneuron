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

#include "NamedDataset.h"

#include <utility>

namespace neoneuron
{

    NamedDataset::NamedDataset(mindset::UID uid, std::string name, mindset::Dataset dataset) :
        _uid(uid),
        _name(std::move(name)),
        _dataset(std::move(dataset))
    {
    }

    mindset::UID NamedDataset::getUID() const
    {
        return _uid;
    }

    const std::string& NamedDataset::getName() const
    {
        return _name;
    }

    void NamedDataset::setName(const std::string& name)
    {
        _name = name;
    }

    mindset::Dataset& NamedDataset::getDataset()
    {
        return _dataset;
    }

    const mindset::Dataset& NamedDataset::getDataset() const
    {
        return _dataset;
    }
} // namespace neoneuron
