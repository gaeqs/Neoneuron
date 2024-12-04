//
// Created by gaeqs on 4/12/24.
//

#ifndef NEONEURONAPPLICATION_H
#define NEONEURONAPPLICATION_H
#include <neoneuron/render/NeoneuronRender.h>

namespace neoneuron {
    class NeoneuronApplication {
    public:
        inline static const std::string CONFIG_FILE = "neoneuron.json";

    private:
        nlohmann::json _settings;
        NeoneuronRender _render;

        hey::Observable<std::string> _settingsNodeChange;

        static nlohmann::json loadSettings();

    public:
        explicit NeoneuronApplication(neon::vulkan::VKApplicationCreateInfo renderCreateInfo);

        [[nodiscard]] NeoneuronRender& getRender();

        [[nodiscard]] const NeoneuronRender& getRender() const;

        [[nodiscard]] nlohmann::json& getSettings();

        [[nodiscard]] const nlohmann::json& getSettings() const;

        void registerSettingsListener(const hey::Listener<std::string>& listener);

        void signalSettingsChange(std::string node);

        void saveSettings() const;
    };
}


#endif //NEONEURONAPPLICATION_H
