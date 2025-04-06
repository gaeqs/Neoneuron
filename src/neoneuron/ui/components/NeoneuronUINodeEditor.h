//
// Created by gaeqs on 2/04/25.
//

#ifndef NEONEURONUINODEEDITOR_H
#define NEONEURONUINODEEDITOR_H

#include <imblueprint/imblueprint.h>
#include <neon/Neon.h>

namespace neoneuron
{
    class NeoneuronUINodeEditor : public neon::Component
    {
        ImBlueprint::Editor _editor;

      public:
        NeoneuronUINodeEditor();

        ~NeoneuronUINodeEditor() override;

        void onUpdate(float deltaTime) override;

        void onPreDraw() override;
    };

} // namespace neoneuron

#endif //NEONEURONUINODEEDITOR_H
