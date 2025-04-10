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

    bool GID::operator==(const GID& other) const
    {
        return datasetId == other.datasetId && internalId == other.internalId;
    }

    bool GID::operator!=(const GID& other) const
    {
        return datasetId != other.datasetId || internalId != other.internalId;
    }
} // namespace neoneuron
