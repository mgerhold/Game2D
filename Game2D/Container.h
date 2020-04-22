#pragma once

#include "GUIComponent.h"
#include <memory>

namespace GUI {

	class Container : public GUIComponent {
	public:
		virtual bool		isSelectable() const;
		virtual bool		handleEvent(const Event& event, const Window& window) override;
		virtual void		draw(const Window& window, RenderStates states) const;
		void				pack(GUIComponent::Ptr component);

	private:
		std::vector<Ptr>	mChildren;
	};

}