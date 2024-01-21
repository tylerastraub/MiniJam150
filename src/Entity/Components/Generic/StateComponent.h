#pragma once

enum class EntityState {
    NOVAL = -1,
    IDLE,
    MOVING,
    JUMPING,
    MINING,
    HURT,
};

struct StateComponent {
    EntityState state = EntityState::NOVAL;
};