#pragma once
#include "IArkanoidPhysics.h"

class PowerUps : public IArkanoidPhysics {
public:
	enum Type{
		Player,
		Lasers,
		Enlarge,
		Catch,
		Slow,
		Break,
		Disruption
	};
public:


private:
	Type type;
};