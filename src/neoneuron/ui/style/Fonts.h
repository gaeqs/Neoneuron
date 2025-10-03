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

#ifndef NEONEURON_FONTS_H
#define NEONEURON_FONTS_H

#include <neon/util/Range.h>
#include <rush/rush.h>

#include <imgui.h>
#include <neon/filesystem/File.h>

namespace neoneuron::fonts
{
    constexpr const char* SS3 = "ss3";

    /**
     * Loads a new font for ImGui to use.
     * These fonts are dynamic; the size of the font can be changed on runtime.
     *
     * @param name The name of the font.
     * @param file The file where the font is stored.
     * @param sizeInPixels The size the loaded font will have.
     * @return Whether the font has been loaded.
     */
    bool loadFont(const std::string& name, const neon::File& file);

    /**
     * Uses the given font as an extra glyph source for the icons that will be loaded next.
     * All previous loaded fonts won't be affected.
     *
     * @param name The name of the font.
     * @param file The file where the font is store.
     * @param loadRange The range of characters that will be loaded.
     * @param offset The offset to apply to the position of each glyph. Used to refine the position of the merged
     * glyphs.
     * @return Whether the font has been loaded.
     */
    bool addMergeFont(const std::string& name, neon::File&& file, neon::Range<ImWchar> loadRange, rush::Vec2f offset);

    std::optional<ImFont*> getFont(const std::string& name);

} // namespace neoneuron::fonts

#endif // NEONEURON_FONTS_H
