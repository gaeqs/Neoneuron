//
// Created by gaeqs on 8/10/24.
//

#include <cmrc/cmrc.hpp>
#include <neon/logging/Logger.h>

#include <neoneuron/ui/NeoneuronRender.h>

CMRC_DECLARE(resources);

int main() {
    auto logger = neon::Logger(true, true, true);
    logger.debug("Hello world!");

    neon::vulkan::VKApplicationCreateInfo info;
    info.name = "Neoneuron";

    neoneuron::NeoneuronRender render(info);

    return render.renderLoop() ? 0 : 1;
}
