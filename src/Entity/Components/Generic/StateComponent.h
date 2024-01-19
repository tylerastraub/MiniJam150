#pragma once

enum class EntityState {
    NOVAL = -1,
    IDLE,
    MOVING,
    JUMPING,
};

struct StateComponent {
    EntityState state = EntityState::NOVAL;
};