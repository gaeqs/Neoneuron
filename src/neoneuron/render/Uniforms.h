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

#ifndef NEONEURON_UNIFORMS_H
#define NEONEURON_UNIFORMS_H

/**
 * Structures used to store uniform data.
 */
namespace neoneuron
{

    struct Matrices
    {
        rush::Mat4f view;
        rush::Mat4f projectionView;
        rush::Mat4f inverseProjection;
        std::array<rush::Plane<float>, 6> planes;
        float near;
        float far;
    };

    struct Scene
    {
        rush::Vec3f sceneCenter;
        float dummy;
        rush::Vec3f sceneRadius;
    };

} // namespace neoneuron

#endif // NEONEURON_UNIFORMS_H
