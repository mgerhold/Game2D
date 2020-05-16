#include "ParallaxController.h"
#include "Entity.h"

ParallaxController::ParallaxController(const Camera& camera, glm::vec2 parallaxFactors)
	: mCamera(camera)
	, mParallaxFactors(parallaxFactors)
{}

void ParallaxController::onUpdate(Time dt) {
	glm::vec2 position = -mCamera.getPosition() * mParallaxFactors;
	getEntity()->setPosition(position);
}
