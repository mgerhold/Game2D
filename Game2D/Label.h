#pragma once

#include "GUIComponent.h"
#include "Text.h"

namespace GUI {

	class Label : public GUIComponent {
	public:
		virtual bool		isSelectable() const;
		virtual void		draw(const Window& window, RenderStates states) const;
		void				setString(const std::string& string);
		void				setFont(Font& font, unsigned int fontHeight);
		virtual glm::ivec2	getSize() const override;

	private:
		Text				mText;

	};

}