//
// Created by gaeqs on 15/11/24.
//

#ifndef ABSTRACTNEURONSELECTOR_H
#define ABSTRACTNEURONSELECTOR_H

namespace neoneuron {
    class AbstractSelector {
    public:
        virtual ~AbstractSelector() = default;

        virtual void setSelectionData(const void* data) = 0;
    };
}

#endif //ABSTRACTNEURONSELECTOR_H
