//
// Created by gaeqs on 4/12/24.
//

#ifndef NEONEURONUISETTINGS_H
#define NEONEURONUISETTINGS_H

#include <string>
#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron::settings {
    void settingsSection(const std::string& name);

    void settingsTheme(NeoneuronApplication* app);

    void settingsFontSize(NeoneuronApplication* app);

    void settingsDialog(NeoneuronApplication* app);
}

#endif //NEONEURONUISETTINGS_H
