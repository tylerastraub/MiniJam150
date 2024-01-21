#pragma once

struct PlayerComponent {
    bool requestsLight = false;
    bool requestsRespawn = false;
    int beaconsLit = 0;
};