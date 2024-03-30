#pragma once

#include <list>

namespace vars {
    float windowWidth = 0, windowHeight = 0, windowRatio = 1;

    namespace events {
        namespace mouse {
            typedef void (*callback) (float px, float py, float lx, float ly, float dx, float dy);
            std::list<callback> callbacks;

            float dx = 0, dy = 0;
        }

        namespace keys {
            typedef void (*callback) (int key, int scancode, int action, int mods);
            std::list<callback> callbacks;

            bool keys[1024];
        }
    }
}