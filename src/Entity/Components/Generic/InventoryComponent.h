#pragma once

#include "ItemType.h"

#include <unordered_map>

struct InventoryComponent {
    std::unordered_map<ItemType, int> inventory;
};