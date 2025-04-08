//
// Created by gaeqs on 2/04/25.
//

#ifndef NEONEURONUINODEEDITOR_H
#define NEONEURONUINODEEDITOR_H

#include <imblueprint/imblueprint.h>
#include <neon/Neon.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/components/nodes/NodeFactory.h>

namespace neoneuron
{
    class NeoneuronUINodeEditor : public neon::Component
    {
        NeoneuronApplication* _application;
        ImBlueprint::Editor _editor;
        std::vector<NodeFactory> _factories;

      public:
        explicit NeoneuronUINodeEditor(NeoneuronApplication* application);

        ~NeoneuronUINodeEditor() override;

        void onUpdate(float deltaTime) override;

        void onPreDraw() override;

        void addFactory(NodeFactory factory);
    };

} // namespace neoneuron

#endif // NEONEURONUINODEEDITOR_H
