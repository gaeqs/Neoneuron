//
// Created by gaeqs on 6/01/25.
//

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
