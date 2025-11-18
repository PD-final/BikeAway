#pragma once
#include "Object.h"
#include "Character.h"

enum class PowerUpType { Coffee, Beer, Shield, StudentID };

class PowerUp : public Object {
public:
    PowerUpType type;

    void apply(Character& player);
};
