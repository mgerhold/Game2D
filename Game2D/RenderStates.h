#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

struct RenderStates {
	Transform transform = Transform();
	const Shader* shader = nullptr;
	const Texture* texture = nullptr;
};