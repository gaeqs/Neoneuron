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
