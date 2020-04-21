#pragma once

#include "Event.h"
#include "Drawable.h"
#include "Transformable.h"
#include "NonCopyable.h"
#include "Window.h"

namespace GUI {

	class Component 
		: public Drawable
		, public Transformable
		, private NonCopyable
	{
	public:
		using Ptr = std::shared_ptr<Component>;

	public:
						Component();
		virtual			~Component() = default;
		virtual bool	isSelectable() const = 0;
		bool			isSelected() const;
		virtual void	select();
		virtual void	deselect();
		virtual void	activate();
		virtual void	deactivate();
		bool			isActive() const;
		virtual bool	handleEvent(const Event& event, const Window& window);
	private:
		bool			mIsSelected;
		bool			mIsActive;
	};

}