#pragma once

#include "Component.h"
#include <string>
#include "Tilemap.h"

class SavegameController : public Component {
public:
				SavegameController();
	void		saveLevel(const std::string& filename) const;
	void		loadLevel(const std::string& filename);
private:		
	void		onAwake() override;
	void		onDraw(const Window& window, RenderStates states) const override;
	void		onUpdate(Time dt) override;

private:
	Tilemap*	mTilemap;
};