#pragma once

#include "State.h"
#include "EntityContainer.h"
#include "Camera.h"
#include "Container.h"
#include "Sprite.h"
#include "Hourglass.h"
#include "Music.h"

class GameState : public State {
public:
					GameState(StateStack* stateStack);
	virtual			~GameState();

	bool			update(Time dt) override;
	void			draw(const Window& window) const override;
	bool			handleEvent(Event e) override;

private:
	void			updateSelectedPreviewTile();
	void			setSelection(int n);
	void			incSelection();
	void			decSelection();

private:
	EntityContainer mEntityContainer;
	Entity*			mTilemap = nullptr;
	Entity*			mPlayer = nullptr;
	Camera			mCamera;
	GUI::Container	mGUIContainer;
	Sprite			mPreviewSprite;
	int				mSelectedPreviewTile = 1;
	Hourglass		mHourglass;
	Music			mMusic;
	bool			mEditMode = false;
};