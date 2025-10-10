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

#include "NeoneuronUIAbout.h"

#include "imgui_impl_vulkan.h"
#include "implot.h"

#include <neoneuron/ui/style/Fonts.h>
#include <neoneuron/application/NeoneuronApplication.h>

#include <mindset/version.h>
#include <neon/version.h>
#include <neoneuron/version.h>

#include <GLFW/glfw3.h>

#if defined(_WIN32)
    #include <Windows.h>
#elif defined(__APPLE__)
    #include <sys/sysctl.h>
#elif defined(__linux__)
    #include <sys/utsname.h>
#endif

CMRC_DECLARE(resources);

namespace neoneuron
{
    std::string NeoneuronUIAbout::getPlatform() const
    {
#ifdef GLFW_ANY_PLATFORM
        switch (glfwGetPlatform()) {
            case GLFW_PLATFORM_X11:
                return "X11";
            case GLFW_PLATFORM_WIN32:
                return "Win32";
            case GLFW_PLATFORM_WAYLAND:
                return "Wayland";
            case GLFW_PLATFORM_COCOA:
                return "Cocoa";
            default:
                return "None";
        }
#else
        return "Cannot fetch, GLFW < 3.4";
#endif
    }

    std::string NeoneuronUIAbout::getOSName() const
    {
#if defined(_WIN32)
        return "Windows";
#elif defined(__APPLE__)
        return "MacOS";
#elif defined(__linux__)
        utsname name;
        if (uname(&name) == 0) {
            return std::format("Linux {}", name.release);
        }
        return "Linux";
#endif
    }

    std::string NeoneuronUIAbout::getLibCName() const
    {
#if defined(__GLIBC__)
        return std::format("GLIBC {} {}", __GLIBC__, __GLIBC_MINOR__);
#else
        return "Unknown";
#endif
    }

    void NeoneuronUIAbout::showIcon() const
    {
        ImVec2 imageSize = ImVec2(ICON_SIZE, ICON_SIZE);
        ImVec2 avail = ImGui::GetContentRegionAvail();
        float offsetX = (avail.x - imageSize.x) * 0.5f;

        for (auto& icon : _icons) {
            if (offsetX > 0.0f) {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
            }
            ImGui::Image(icon->getImGuiDescriptor(), imageSize);
        }
    }

    void NeoneuronUIAbout::showTitle() const
    {
        ImGui::PushFont(nullptr, 32.0f);
        ImGui::Text("Neoneuron %s", NEONEURON_VERSION);
        ImGui::PopFont();
    }

    void NeoneuronUIAbout::showVersionInfo() const
    {
        ImGui::PushFont(nullptr, 24.0f);
        ImGui::Text("Build %s, built on %s", NEONEURON_GIT_COMMIT, NEONEURON_BUILD_DATE);
        ImGui::PopFont();
    }

    void NeoneuronUIAbout::showAcknowledges() const
    {
        ImGui::PushFont(nullptr, 20.0f);
        ImGui::Text("Gael Rial Costas - VG-Lab/URJC © 2025-%d", NEONEURON_BUILD_YEAR);
        ImGui::PopFont();
    }

    void NeoneuronUIAbout::showLibraries() const
    {
        ImGui::Text("Libraries:");
        ImGui::BulletText("Neon %s, build %s", NEON_VERSION, NEON_GIT_COMMIT);
        ImGui::BulletText("Mindset %s, build %s", MINDSET_VERSION, MINDSET_GIT_COMMIT);
#ifdef BRION_REV_STRING
        ImGui::BulletText("Brion %s", BRION_REV_STRING);
#else
        ImGui::BulletText("Brion not found");
#endif
        ImGui::BulletText("ImGui %s", IMGUI_VERSION);
        ImGui::BulletText("ImPlot %s", IMPLOT_VERSION);
        ImGui::BulletText("Vulkan %d.%d.%d", VK_VERSION_MAJOR(_vulkanAPIVersion), VK_VERSION_MINOR(_vulkanAPIVersion),
                          VK_VERSION_PATCH(_vulkanAPIVersion));
        ImGui::BulletText("GLFW %s", _glfwVersion.c_str());
    }

    void NeoneuronUIAbout::showEnvironment() const
    {
        ImGui::Text("Environment:");
        ImGui::BulletText("OS: %s", getOSName().c_str());
        ImGui::BulletText("Libc: %s", getLibCName().c_str());
        ImGui::BulletText("Display platform: %s", getPlatform().c_str());
    }

    void NeoneuronUIAbout::showLicense() const
    {
        ImGui::Text("License:");
        ImGui::TextWrapped("%s", _license.c_str());
    }

    void NeoneuronUIAbout::loadIcons(NeoneuronApplication* application)
    {
        constexpr const char* ICONS[] = {"icon/256x256/neoneuron.png", "icon/vglab.png", "icon/urjc.png"};
        auto* app = &application->getRender().getApplication();
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        _icons.reserve(std::size(ICONS));
        for (const char* icons : ICONS) {
            if (auto file = fs.readFile(icons)) {
                auto& f = file.value();
                std::string id = std::format("neoneuron:icon_{}", icons);
                auto texture = neon::Texture::createTextureFromFile(app, id, f.getData(), f.getSize());
                _icons.push_back(neon::SampledTexture::create(app, texture));
            }
        }
    }

    NeoneuronUIAbout::NeoneuronUIAbout(NeoneuronApplication* application) :
        _open(false)
    {
        loadIcons(application);

        vkEnumerateInstanceVersion(&_vulkanAPIVersion);
        _glfwVersion = std::string(glfwGetVersionString());

        const char* version_string = glfwGetVersionString();
        _glfwVersion = version_string;

        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        auto licenseFile = fs.readFile("LICENSE.txt");
        if (licenseFile.has_value()) {
            _license = std::string(reinterpret_cast<const char*>(licenseFile.value().getData()));
        }
    }

    NeoneuronUIAbout::~NeoneuronUIAbout()
    {
    }

    void NeoneuronUIAbout::onPreDraw()
    {
        if (!_open) {
            ImGui::OpenPopup("About Neoneuron");
            _open = true;
        }

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(600.0f, 400.0f), ImGuiCond_Always);
        if (ImGui::BeginPopupModal("About Neoneuron", &_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
            ImGui::BeginChild("neoneuron:about_sidebar", ImVec2(SIDEBAR_SIZE, 0), ImGuiChildFlags_None,
                              ImGuiWindowFlags_NoBackground);
            showIcon();
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("neoneuron:about_main", ImVec2(0, 0), ImGuiChildFlags_None,
                              ImGuiWindowFlags_NoBackground);
            ImGui::Dummy(ImVec2(0, 16.0f));
            showTitle();
            showVersionInfo();
            showAcknowledges();
            ImGui::Separator();
            ImGui::Spacing();
            showLibraries();
            showEnvironment();

            ImGui::Separator();
            ImGui::Spacing();
            showLicense();

            ImGui::EndChild();
            ImGui::EndPopup();
        }

        if (!_open) {
            getGameObject()->destroy();
        }
    }
} // namespace neoneuron