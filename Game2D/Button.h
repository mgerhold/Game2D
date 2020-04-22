#pragma once

#include "GUIComponent.h"
#include "Text.h"
#include "Sprite.h"
#include <functional>

namespace GUI {

	class Button : public GUIComponent {
	public:
		using CallbackFunc = std::function<void(void)>;

	public:
							Button();

		virtual bool		isSelectable() const;
		virtual bool		handleEvent(const Event& event, const Window& window) override;
		virtual void		draw(const Window& window, RenderStates states) const;
		void				setString(const std::string& string);
		void				setFont(Font& font, unsigned int fontHeight);
		virtual glm::ivec2	getSize() const override;
		void				setNormalTexture(const Texture& texture);
		void				setSelectedTexture(const Texture& texture);
		void				setActiveTexture(const Texture& texture);
		void				setCallbackFunc(CallbackFunc func);		

	private:
		enum class State {
			Normal,
			Selected,
			Active,
		};

	private:
		void				updateSprite();
		void				centerText();
		bool				isMouseInside(const Event& event, const Window& window) const;

	private:
		Text				mText;
		Sprite				mSprite;
		const Texture*		mTextureNormal;
		const Texture*		mTextureSelected;
		const Texture*		mTextureActive;
		bool				mIsHeldDown;
		CallbackFunc		mCallbackFunc;

	};

}