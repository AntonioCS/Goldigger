#pragma once

#include <utility>

namespace Acs {
	using Point = std::pair<int, int>;
	using Dimension = std::pair<int, int>;

	class Entity
	{
		int m_map_x;
		int m_map_y;
		int m_map_w;
		int m_map_h;

		int m_sheet_x;
		int m_sheet_y;
		int m_sheet_w;
		int m_sheet_h;
	public:
		Entity();
		virtual ~Entity();

		void setLocation(int x, int y) noexcept;
		Point getLocation() const noexcept;
		Dimension getDimension() const noexcept;

		void move(float dx, float dy);

		void setX(int x) {
			m_map_x = x;
		}

		void setY(int y) {
			m_map_y = y;
		}
	};
}