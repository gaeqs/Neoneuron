//
// Created by gaeqs on 8/10/24.
//

#include <neon/logging/Logger.h>

int main() {
    auto logger = neon::Logger(true, true, true);
    logger.debug("Hello world!");
    return 0;
}
