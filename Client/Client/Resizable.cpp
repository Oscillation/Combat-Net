#include "Resizable.h"

Resizable::Resizable() :
	pressed(false),
	intersecting(false),
	move(false),
	resize(false),
	up(false),
	down(false),
	left(false),
	right(false)
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
	resize(false),
	up(false),
	down(false),
	left(false),
	right(false)
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
	if (move)
	{
		sf::Vector2<int> delta = sf::Vector2<int>(m_position.x - sf::Mouse::getPosition(p_window).x, m_position.y - sf::Mouse::getPosition(p_window).y);

		m_position = sf::Mouse::getPosition(p_window);

		resizable_ptr_bounds->left -= delta.x;
		resizable_ptr_bounds->top -= delta.y;

		setSides();

	}else if(resize)
	{

		if (up)
		{
			int delta = m_position.y - sf::Mouse::getPosition(p_window).y;
			resizable_ptr_bounds->top -= delta;
			resizable_ptr_bounds->height += delta;

			setSides();
		}else if (down)
		{
			int delta = sf::Mouse::getPosition(p_window).y - m_position.y;

			if (resizable_ptr_bounds->height + delta > 10)
			{
				resizable_ptr_bounds->height += delta;

			}else{
				resizable_ptr_bounds->height = 10;
			}

			setSides();
		}

		if (left)
		{
			int delta = m_position.x - sf::Mouse::getPosition(p_window).x;
			resizable_ptr_bounds->left -= delta;
			resizable_ptr_bounds->width += delta;

			setSides();
		}else if (right)
		{
			int delta = sf::Mouse::getPosition(p_window).x - m_position.x;

			if (resizable_ptr_bounds->width + delta > 10)
			{
				resizable_ptr_bounds->width += delta;

			}else{
				resizable_ptr_bounds->width = 10;
			}

			setSides();
		}

		m_position = sf::Mouse::getPosition(p_window);
	}

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

				up = sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[0]);
				down = sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[1]);
				left = sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[2]);
				right = sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[3]);
			}
		}
		pressed = true;
	}else
	{
		move = false;
		resize = false;
		pressed = false;
	}

	if (!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(*resizable_ptr_bounds) && 
		!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[0]) && 
		!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[1]) && 
		!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[2]) && 
		!sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(m_sides[3]) &&
		!pressed)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_sides[i] = sf::Rect<int>(0, 0, 0, 0);
		}
		intersecting = false;
	}

	if (sf::Rect<int>(sf::Mouse::getPosition(p_window).x, sf::Mouse::getPosition(p_window).y, 1, 1).intersects(*resizable_ptr_bounds))
	{
		setSides();
		intersecting = true;
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
