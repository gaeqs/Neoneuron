//
// Created by gaeqs on 31/03/25.
//

#ifndef GPUSYNAPSECREATEINFO_H
#define GPUSYNAPSECREATEINFO_H

namespace neoneuron
{

    struct GPUSynapseCreateInfo
    {
        mindset::Dataset* dataset;
        neon::InstanceData* instanceData;
        mindset::UID prePositionProperty;
        mindset::UID postPositionProperty;
    };

} // namespace neoneuron

#endif //GPUSYNAPSECREATEINFO_H
