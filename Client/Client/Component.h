#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\System\NonCopyable.hpp>
#include <SFML\Graphics\Transformable.hpp>

#include <memory>

namespace sf
{
	class Event;
}

namespace gui
{

	class Component : 
		public sf::Drawable, 
		public sf::Transformable, 
		private sf::NonCopyable
	{

		public:
			typedef std::unique_ptr<Component> ptr;

		public:
							Component();
			virtual			~Component();
			
			virtual bool	isSelectable() const = 0;
			bool			isSelected() const;
			virtual void	select();
			virtual void	deselect();

			virtual bool	isActive() const;
			virtual void	activate();
			virtual void	deactivate();

			virtual void	handleEvent(sf::Event const& event) = 0;

		private:

		private:
			bool			mActive;
			bool			mSelected;
						
	};

}