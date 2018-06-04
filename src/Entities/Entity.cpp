#include "Entity.h"

namespace Acs {

	Entity::Entity()
	{
	}

	Entity::~Entity()
	{
	}

	void Entity::setLocation(int x, int y) noexcept
	{
		m_map_x = x;
		m_map_y = y;
	}

	Point Entity::getLocation() const noexcept
	{
		return { m_map_x, m_map_y };
	}
	Dimension Entity::getDimension() const noexcept
	{
		return { m_map_w, m_map_h };
	}
	void Entity::move(float dx, float dy) 	
	{
		m_map_x += dx;
		m_map_y += dy;
	}
}