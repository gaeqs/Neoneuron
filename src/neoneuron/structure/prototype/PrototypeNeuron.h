//
// Created by gaeqs on 14/11/24.
//

#ifndef PROTOTYPENEURON_H
#define PROTOTYPENEURON_H

#include <string>
#include <unordered_map>
#include <vector>
#include <neoneuron/structure/Identifiable.h>

#include "PrototypeNeuronSegment.h"

namespace neoneuron {
    class PrototypeNeuron : public Identifiable {
        std::unordered_map<std::string, UID> _properties;
        std::vector<PrototypeNeuronSegment> _segments;
        std::unordered_map<UID, std::any> _globalProperties;

    public:
        explicit PrototypeNeuron(UID id);

        UID defineProperty(std::string name);

        void defineProperty(std::string name, UID id);

        [[nodiscard]] std::optional<UID> getPropertyUID(const std::string& name) const;

        [[nodiscard]] std::unordered_map<std::string, UID>& getPropertiesUID();

        [[nodiscard]] const std::unordered_map<std::string, UID>& getPropertiesUID() const;

        void addSegment(PrototypeNeuronSegment segment);

        [[nodiscard]] std::vector<PrototypeNeuronSegment>& getSegments();

        [[nodiscard]] const std::vector<PrototypeNeuronSegment>& getSegments() const;

        void reserveSpaceForSegments(size_t space);

        void setPropertyAny(UID uid, std::any value);

        template<typename T>
        void setProperty(UID uid, const T& value) {
            setPropertyAny(uid, value);
        }

        [[nodiscard]] std::optional<std::any> getPropertyAsAny(UID uid) const;

        template<typename T>
        [[nodiscard]] std::optional<T> getProperty(UID uid) const {
            auto optional = getPropertyAsAny(uid);
            if (!optional.has_value()) return {};
            try {
                return std::any_cast<T>(optional.value());
            } catch (const std::bad_any_cast& e) {
                return {};
            }
        }

        template<typename T>
        [[nodiscard]] std::optional<T> getProperty(const std::string& name) const {
            auto uid = getPropertyUID(name);
            if (!uid.has_value()) return {};
            return getProperty<T>(uid.value());
        }
    };
}


#endif //PROTOTYPENEURON_H
