//
// Created by gaeqs on 8/10/24.
//

#include <cmrc/cmrc.hpp>
#include <neon/logging/Logger.h>

#include "loader/SWCLoader.h"
#include "structure/Neuron.h"
#include "structure/NeuronSegment.h"

CMRC_DECLARE(resources);

int main() {
    auto logger = neon::Logger(true, true, true);
    logger.debug("Hello world!");

    auto file = cmrc::resources::get_filesystem().open("data.swc");
    std::string fileData(file.begin(), file.end());
    std::stringstream ss(fileData);
    neoneuron::SWCLoader loader(ss);

    auto result = loader.build(1);

    if (!result.isOk()) {
        logger.error(result.getError());
        return 0;
    }

    neoneuron::Neuron neuron = std::move(result.getResult());

    for (auto& segment: neuron.getSegments()) {
        logger.debug(neon::MessageBuilder().print("Segment: ").print(segment.getId()));
        logger.debug(neon::MessageBuilder().print(" - Type: ").print((uint8_t)segment.getType()));
        logger.debug(neon::MessageBuilder().print(" - Start: ").print(segment.getStart()));
        logger.debug(neon::MessageBuilder().print(" - End: ").print(segment.getEnd()));
        logger.debug(neon::MessageBuilder().print(" - Radius: ").print(segment.getRadius()));
        logger.debug(neon::MessageBuilder().print(" - Parent: ").print(segment.getParentId().value_or(-1)));
    }

    return 0;
}
