//
// Created by gaeqs on 14/11/24.
//

#ifndef PROTOTYPENEURONSEGMENT_H
#define PROTOTYPENEURONSEGMENT_H

#include <any>
#include <optional>
#include <unordered_map>
#include <neon/logging/Logger.h>
#include <neoneuron/structure/Identifiable.h>

namespace neoneuron {
    class PrototypeNeuronSegment : public Identifiable {
        std::unordered_map<UID, std::any> _properties;

    public:
        PrototypeNeuronSegment(const PrototypeNeuronSegment& other) = default;

        PrototypeNeuronSegment(PrototypeNeuronSegment&& other) noexcept;

        PrototypeNeuronSegment& operator=(const PrototypeNeuronSegment& other);

        PrototypeNeuronSegment& operator=(PrototypeNeuronSegment&& other) noexcept;

        [[nodiscard]] const std::unordered_map<UID, std::any>& getProperties() const;

        explicit PrototypeNeuronSegment(UID uid);

        void setPropertyAny(UID uid, std::any value);

        template<typename T>
        void setProperty(UID uid, const T& value) {
            setPropertyAny(uid, value);
        }

        std::optional<std::any> getPropertyAsAny(UID uid) const;

        template<typename T>
        std::optional<T> getProperty(UID uid) const {
            auto optional = getPropertyAsAny(uid);
            if (!optional.has_value()) return {};
            auto any = std::move(optional.value());
            try {
                return std::any_cast<T>(any);
            } catch (const std::bad_any_cast& e) {
                neon::error() << "Error while casting std::any. Expected: "
                        << typeid(T).name() << ", Found: " << any.type().name();
                return {};
            }
        }
    };
}


#endif //PROTOTYPENEURONSEGMENT_H
