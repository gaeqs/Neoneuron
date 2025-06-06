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

#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <unordered_map>
#include <optional>

namespace neoneuron
{
    template<typename Type, typename Key = std::string>
    class Storage
    {
        std::unordered_map<Key, Type> _data;

      public:
        Storage() = default;

        bool contains(const Key& key)
        {
            return _data.contains(key);
        }

        std::optional<const Type*> get(const Key& key) const
        {
            auto it = _data.find(key);
            if (it == _data.end()) {
                return {};
            }
            return {&it->second};
        }

        std::optional<Type*> get(const Key& key)
        {
            auto it = _data.find(key);
            if (it == _data.end()) {
                return {};
            }
            return {&it->second};
        }

        bool add(const Key& key, Type type)
        {
            return _data.insert({key, std::move(type)}).second;
        }

        bool remove(const Key& key)
        {
            return _data.erase(key) > 0;
        }

        auto begin()
        {
            return _data.begin();
        }

        auto begin() const
        {
            return _data.begin();
        }

        auto end()
        {
            return _data.end();
        }

        auto end() const
        {
            return _data.end();
        }
    };
} // namespace neoneuron

#endif //STORAGE_H
