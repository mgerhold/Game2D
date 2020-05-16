#pragma once

#include "Component.h"
#include "Camera.h"
#include <glm/vec2.hpp>

class ParallaxController : public Component {
public:
	ParallaxController(const Camera& camera, glm::vec2 parallaxFactors);

private:
	void onUpdate(Time dt) override;

private:
	const Camera& mCamera;
	glm::vec2 mParallaxFactors;
};