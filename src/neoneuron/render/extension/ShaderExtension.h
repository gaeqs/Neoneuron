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

//
// Created by gaeqs on 10/10/25.
//

#ifndef NEONEURON_SHADEREXTENSION_H
#define NEONEURON_SHADEREXTENSION_H

#include <neon/render/shader/ShaderUniformBuffer.h>
#include <neon/render/shader/ShaderUniformDescriptor.h>

#include <string>
#include <memory>
#include <unordered_map>

namespace neoneuron
{
    template<typename Identification>
    class ShaderExtension
    {
      protected:
        std::unordered_map<Identification, std::pair<uint32_t, size_t>> _idToIndex;

        virtual uint32_t newInstance() = 0;

        virtual void deleteInstance(uint32_t id) = 0;

      public:
        virtual ~ShaderExtension() = default;

        [[nodiscard]] virtual std::string generateShaderCode(size_t uniformSet) const = 0;

        [[nodiscard]] virtual std::optional<std::shared_ptr<neon::ShaderUniformDescriptor>> getUBODescriptor()
            const = 0;

        [[nodiscard]] virtual std::optional<std::shared_ptr<neon::ShaderUniformBuffer>> getUBO() const = 0;

        [[nodiscard]] virtual std::string getExtensionName() const = 0;

        uint32_t registerElement(Identification id)
        {
            auto it = _idToIndex.find(id);
            if (it != _idToIndex.end()) {
                ++it->second.second;
                return it->second.first;
            }

            auto index = newInstance();
            _idToIndex.emplace(id, std::pair<uint32_t, size_t>(index, 1u));
            return index;
        }

        bool unregisterElement(Identification id)
        {
            auto it = _idToIndex.find(id);
            if (it != _idToIndex.end()) {
                --it->second.second;
                if (it->second.second == 0) {
                    deleteInstance(it->second.first);
                    _idToIndex.erase(it);
                }
                return true;
            }
            return false;
        }
    };
} // namespace neoneuron

#endif // NEONEURON_SHADEREXTENSION_H
