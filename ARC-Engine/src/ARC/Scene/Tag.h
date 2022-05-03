#pragma once
#include "ARC\Objects\Ecs.h"

namespace ARC
{
	struct InvalidEntityTag{};
	struct EntityTag{};

	RegisterTag(InvalidEntityTag);
	RegisterTag(EntityTag);
	
}