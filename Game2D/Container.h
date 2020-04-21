#pragma once

#include "Component.h"
#include <memory>

namespace GUI {

	class Container : public Component {
	public:
		virtual bool		isSelectable() const;
		virtual bool		handleEvent(const Event& event, const Window& window) override;
		virtual void		draw(const Window& window, RenderStates states) const;
		void				pack(Component::Ptr component);

	private:
		std::vector<Ptr>	mChildren;
	};

}