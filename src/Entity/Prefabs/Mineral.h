#include "SpritesheetPropertiesComponent.h"
#include "MineralComponent.h"

#include <entt/entity/registry.hpp>

namespace prefab {
    class Mineral {
    public:
        Mineral() = default;
        ~Mineral() = default;

        static entt::entity create(entt::registry& ecs);
        static entt::entity create(entt::registry& ecs, strb::vec2f pos, MineralType mineralType);

    private:
        static SpritesheetPropertiesComponent createSpritesheetPropertiesComponent(Spritesheet* spritesheet);
        static float getMineralMineSpeed(MineralType mineralType);

    };
}