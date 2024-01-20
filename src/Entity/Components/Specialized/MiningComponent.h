#pragma once

#include "rect2.h"

struct MiningComponent {
    strb::rect2f collisionRect = {0.f, 0.f, 16.f, 32.f};
    bool canMine = true;
    bool isMining = false;
};