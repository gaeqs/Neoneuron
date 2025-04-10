//
// Created by gaeqs on 09/04/2025.
//

#include "GID.h"

namespace neoneuron
{

    GID::GID() :
        datasetId(0),
        internalId(0)
    {
    }

    GID::GID(mindset::UID datasetId_, mindset::UID internalId_) :
        datasetId(datasetId_),
        internalId(internalId_)
    {
    }
} // namespace neoneuron
