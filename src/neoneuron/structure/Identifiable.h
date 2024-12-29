//
// Created by gaeqs on 8/10/24.
//

#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

#include <neoneuron/Types.h>


namespace neoneuron {
    class Identifiable {
        UID _id;

    public:
        explicit Identifiable(UID id);

        [[nodiscard]] UID getId() const;

        void setId(UID id);
    };
}

#endif //IDENTIFIABLE_H
