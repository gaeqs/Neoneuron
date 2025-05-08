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

#ifndef NEONEURONAPPLICATION_H
#define NEONEURONAPPLICATION_H

#include <mindset/loader/LoaderRegistry.h>

#include <neoneuron/filesystem/NeoneuronFiles.h>
#include <neoneuron/loader/LoaderCollection.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/structure/Repository.h>
#include <neoneuron/structure/Selector.h>
#include <neoneuron/structure/Storage.h>
#include <neoneuron/structure/property/DefinedProperty.h>

namespace neoneuron
{

    /**
     * This is the main class of Neoneuron.
     * The application should only have one instance of this class.
     */
    class NeoneuronApplication
    {
        NeoneuronFiles _files;
        Repository _repository;
        Selector _selector;
        NeoneuronRender _render;
        Storage<DefinedProperty> _propertyStorage;
        mindset::LoaderRegistry _loaderStorage;
        LoaderCollection _loaderCollection;

      public:
        NeoneuronApplication(const NeoneuronApplication&) = delete;

        explicit NeoneuronApplication(const neon::vulkan::VKApplicationCreateInfo& renderCreateInfo);

        /**
         *
         * Returns the class that manages the files of the application.
         *
         * @result The file manager of the application.
         */
        [[nodiscard]] NeoneuronFiles& getFiles();

        /**
         *
         * Returns the class that manages the files of the application.
         *
         * @result The file manager of the application.
         */
        [[nodiscard]] const NeoneuronFiles& getFiles() const;

        /**
         * The repository where all the neuronal information is stored.
         * Insert your brain data here!
         */
        [[nodiscard]] Repository& getRepository();

        /**
         * The repository where all the neuronal information is stored.
         * Insert your brain data here!
         */
        [[nodiscard]] const Repository& getRepository() const;

        /**
         * The selection of the application.
         */
        [[nodiscard]] Selector& getSelector();

        /**
         * The selection of the application.
         */
        [[nodiscard]] const Selector& getSelector() const;

        /**
         * This class manages the render part of the application.
         */
        [[nodiscard]] NeoneuronRender& getRender();

        /**
         * This class manages the render part of the application.
         */
        [[nodiscard]] const NeoneuronRender& getRender() const;

        /**
         * Storage used to store all properties' definitions.
         * If you want to define a new property that can be saved, loaded and edited,
         * you should store its definition here!
         */
        Storage<DefinedProperty>& getPropertyStorage();

        /**
         * Storage used to store all properties' definitions.
         * If you want to define a new property that can be saved, loaded and edited,
         * you should store its definition here!
         */
        const Storage<DefinedProperty>& getPropertyStorage() const;

        /**
         * This registry is used to register loaders Neoneuron can use to load scenes.
         * If you have created a new loader, you should register it here!
         */
        mindset::LoaderRegistry& getLoaderRegistry();

        /**
         * This registry is used to register loaders Neoneuron can use to load scenes.
         * If you have created a new loader, you should register it here!
         */
        const mindset::LoaderRegistry& getLoaderRegistry() const;

        /**
         * This collection is used to manage and storage running loaders.
         * Using this collection, you can retrieve the status of the
         * currently loading datasets.
         */
        LoaderCollection& getLoaderCollection();

        /**
         * This collection is used to manage and storage running loaders.
         * Using this collection, you can retrieve the status of the
         * currently loading datasets.
         */
        const LoaderCollection& getLoaderCollection() const;
    };
} // namespace neoneuron

#endif // NEONEURONAPPLICATION_H
