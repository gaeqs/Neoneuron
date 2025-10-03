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

#include "NeoneuronApplication.h"
#include "NeoneuronDefaults.h"

namespace neoneuron
{

    NeoneuronApplication::NeoneuronApplication(const neon::vulkan::VKApplicationCreateInfo& renderCreateInfo) :
        _selector(&_repository),
        _render(this, renderCreateInfo),
        _loaderCollection(this)
    {
        initDefaultProperties(_propertyStorage);
    }

    NeoneuronFiles& NeoneuronApplication::getFiles()
    {
        return _files;
    }

    const NeoneuronFiles& NeoneuronApplication::getFiles() const
    {
        return _files;
    }

    Repository& NeoneuronApplication::getRepository()
    {
        return _repository;
    }

    const Repository& NeoneuronApplication::getRepository() const
    {
        return _repository;
    }

    Selector& NeoneuronApplication::getSelector()
    {
        return _selector;
    }

    const Selector& NeoneuronApplication::getSelector() const
    {
        return _selector;
    }

    NeoneuronRender& NeoneuronApplication::getRender()
    {
        return _render;
    }

    const NeoneuronRender& NeoneuronApplication::getRender() const
    {
        return _render;
    }

    Storage<DefinedProperty>& NeoneuronApplication::getPropertyStorage()
    {
        return _propertyStorage;
    }

    const Storage<DefinedProperty>& NeoneuronApplication::getPropertyStorage() const
    {
        return _propertyStorage;
    }

    mindset::LoaderRegistry& NeoneuronApplication::getLoaderRegistry()
    {
        return _loaderStorage;
    }

    const mindset::LoaderRegistry& NeoneuronApplication::getLoaderRegistry() const
    {
        return _loaderStorage;
    }

    LoaderCollection& NeoneuronApplication::getLoaderCollection()
    {
        return _loaderCollection;
    }

    const LoaderCollection& NeoneuronApplication::getLoaderCollection() const
    {
        return _loaderCollection;
    }

} // namespace neoneuron
