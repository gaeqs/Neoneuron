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

#ifndef NEONEURONRENDERDATA_H
#define NEONEURONRENDERDATA_H
#undef max

namespace neoneuron
{
    struct NeoneuronRenderData
    {
        float currentTime = 0.0f; // Value only valid on GPU. use Render::getCurrentTime() instead.
        float radiusStrength = 1.0f;
        float somaRadiusStrength = 1.0f;
        float startClip = 0.2f;
        float endClip = 0.1f;
        float splitHeight = 0.3f;
        float splitArcStrength = 0.15f;
        uint32_t rotationIndexOffset = 0;
        uint32_t childrenRotationIndexOffset = 0;
        uint32_t minChildrenForJoint = 2;
        uint32_t verticesPerCircle = 16;
        uint32_t somaLatitudes = 32;
        uint32_t somaLongitudes = 32;
        float somaConnectionPushFactor = 0.3f;
        float somaWeightPower = 3.0f;
        float somaSphereWeight = 0.1f;
        float somaConnectionMaxWeight = 1.0f;
        uint32_t lod = 0;
        uint32_t frame = 0;
        uint32_t savingNeuron = std::numeric_limits<uint32_t>::max();
        rush::Vec4f defaultColor = rush::Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
        rush::Vec4f selectedColor = rush::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
    };
} // namespace neoneuron
#endif //NEONEURONRENDERDATA_H
