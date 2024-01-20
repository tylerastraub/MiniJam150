#pragma once

enum class EntityState {
    NOVAL = -1,
    IDLE,
    MOVING,
    JUMPING,
    MINING,
};

struct StateComponent {
    EntityState state = EntityState::NOVAL;
};