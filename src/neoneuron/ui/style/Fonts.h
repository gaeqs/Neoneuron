//
// Created by gaeqs on 11/11/2024.
//

#ifndef FONTS_H
#define FONTS_H
#include <imgui.h>
#include <neon/filesystem/File.h>

namespace neoneuron::fonts
{
    constexpr const char* SS3_16 = "ss3_16";
    constexpr const char* SS3_18 = "ss3_18";
    constexpr const char* SS3_20 = "ss3_20";
    constexpr const char* SS3_24 = "ss3_24";
    constexpr const char* SS3_32 = "ss3_32";

    bool loadFont(const std::string& name, const neon::File& file, float sizeInPixels);

    std::optional<ImFont*> getFont(const std::string& name);

    void recreateFonts();

    void imGuiPushFont(const std::string& name);
} // namespace neoneuron::fonts

#endif //FONTS_H
