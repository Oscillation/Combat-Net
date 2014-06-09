#include "Resizable.h"

Resizable::Resizable() :
	pressed(false),
	intersecting(false),
	move(false),
	resize(false)
{
	for (int i = 0; i < 4; ++i)
	{
		m_sides[i] = sf::Rect<int>(0, 0, 0, 0);
	}
}

Resizable::Resizable(sf::Rect<int>* ptr_bounds) :
	resizable_ptr_bounds(ptr_bounds),
	pressed(false),
	intersecting(false),
	move(false),
	resize(false)
{
	for (int i = 0; i < 4; ++i)
	{
		m_sides[i] = sf::Rect<int>(0, 0, 0, 0);
	}
}

Resizable::~Resizable()
{

}

void Resizable::update(const sf::RenderWindow & p_window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(*resizable_ptr_bounds))
		{
			if (!pressed)
			{
				m_position = sf::Mouse::getPosition(p_window);

				move = true;
				resize = false;
			}
		}else if(sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[0]) || 
			sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[1]) || 
			sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[2]) || 
			sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[3]))
		{
			if (!pressed)
			{
				m_position = sf::Mouse::getPosition(p_window);
				pressed = true;
				move = false;
				resize = true;
			}
		}
		pressed = true;
	}else
	{
		pressed = false;
	}

	if (intersecting)
	{
		if (!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(*resizable_ptr_bounds) && 
			!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[0]) && 
			!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[1]) && 
			!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[2]) && 
			!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[3]))
		{
			for (int i = 0; i < 4; ++i)
			{
				m_sides[i] = sf::Rect<int>(0, 0, 0, 0);
			}
			intersecting = false;
		}


		if (pressed)
		{
			if (move)
			{
				sf::Vector2<int> delta = sf::Vector2<int>(m_position.x - sf::Mouse::getPosition(p_window).x, m_position.y - sf::Mouse::getPosition(p_window).y);

				m_position = sf::Mouse::getPosition(p_window);

				resizable_ptr_bounds->left -= delta.x;
				resizable_ptr_bounds->top -= delta.y;

				setSides();

			}else if(resize)
			{
				if (sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[0]))
				{
					int delta = m_position.y - sf::Mouse::getPosition(p_window).y;
					resizable_ptr_bounds->top -= delta;
					resizable_ptr_bounds->height += delta;
				}

				if (sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[1]))
				{
					int delta = sf::Mouse::getPosition(p_window).y - m_position.y;
					resizable_ptr_bounds->height += delta;
				}

				if (sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[2]))
				{
					int delta = m_position.x - sf::Mouse::getPosition(p_window).x;
					resizable_ptr_bounds->left -= delta;
					resizable_ptr_bounds->width += delta;
				}

				if (sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[3]))
				{
					int delta = sf::Mouse::getPosition(p_window).x - m_position.x;
					resizable_ptr_bounds->width += delta;
				}
				
				setSides();
				m_position = sf::Mouse::getPosition(p_window);
			}


		}
	}else if (sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(*resizable_ptr_bounds))
	{
		setSides();
		intersecting = true;
	}else
	{
		for (int i = 0; i < 4; ++i)
		{
			m_sides[i] = sf::Rect<int>(0, 0, 0, 0);
		}
	}
}

void Resizable::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const
{
	sf::RectangleShape rect[4] = {
		sf::RectangleShape(sf::Vector2<float>(m_sides[0].width, m_sides[0].height)),
		sf::RectangleShape(sf::Vector2<float>(m_sides[1].width, m_sides[1].height)),
		sf::RectangleShape(sf::Vector2<float>(m_sides[2].width, m_sides[2].height)),
		sf::RectangleShape(sf::Vector2<float>(m_sides[3].width, m_sides[3].height))
	};

	for (int i = 0; i < 4; ++i)
	{
		rect[i].setPosition(m_sides[i].left, m_sides[i].top);
		rect[i].setFillColor(sf::Color(100, 0, 100, 200));

		p_target.draw(rect[i]);
	}
}

void Resizable::setSides()
{
	m_sides[0] = sf::Rect<int>(resizable_ptr_bounds->left - 10, resizable_ptr_bounds->top - 10, resizable_ptr_bounds->width + 20, 10);
	m_sides[1] = sf::Rect<int>(resizable_ptr_bounds->left - 10, resizable_ptr_bounds->top + resizable_ptr_bounds->height, resizable_ptr_bounds->width + 20, 10);

	m_sides[2] = sf::Rect<int>(resizable_ptr_bounds->left - 10, resizable_ptr_bounds->top - 10, 10, resizable_ptr_bounds->height + 20);
	m_sides[3] = sf::Rect<int>(resizable_ptr_bounds->left + resizable_ptr_bounds->width, resizable_ptr_bounds->top - 10, 10, resizable_ptr_bounds->height + 20);
}
