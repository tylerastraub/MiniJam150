#pragma once

#include "ScriptComponent.h"
#include "ItemType.h"

#include <entt/entity/entity.hpp>

struct ItemPickupComponent {
    std::shared_ptr<IScript> onPickupScript = nullptr;
    ItemType itemType = ItemType::NOVAL;
    entt::entity pickedUpBy = entt::null;
};