//
// Created by gaeqs on 09/04/2025.
//

#ifndef NEURONUID_H
#define NEURONUID_H

#include <xhash>
#include <mindset/UID.h>

namespace neoneuron
{

    /**
     * A neural element's global identifier.
     * This struct combines the UID of the dataset with the UID
     * of the target element inside the target dataset.
     *
     * For simplicity, dataset UIDs should not be reused
     * when a dataset is destroyed. This is managed automatically
     * by a Repository.
     */
    struct GID
    {
        mindset::UID datasetId;
        mindset::UID internalId;

        GID();

        GID(mindset::UID datasetId, mindset::UID internalId);

        bool operator==(const GID& other) const;

        bool operator!=(const GID& other) const;
    };

} // namespace neoneuron

template<>
struct std::hash<neoneuron::GID>
{
    size_t operator()(const neoneuron::GID& gid) const noexcept
    {
        size_t h1 = std::hash<mindset::UID>{}(gid.datasetId);
        size_t h2 = std::hash<mindset::UID>{}(gid.internalId);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

#endif // NEURONUID_H
