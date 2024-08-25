#pragma once
#include <array>
#include <memory>
#include <vector>
#include <Core/Vector2.h>

namespace LLGP
{
	class Collider;

	class QuadTree
	{
	public:

		QuadTree(int _threshold, int maxDepth, LLGP::Vector2f& min, LLGP::Vector2f& max);

		void Insert(LLGP::Collider* c);
		std::vector<LLGP::Collider*> GetCollidersInArea(LLGP::Vector2f center, LLGP::Vector2f extents);

	private:
		struct Cell
		{
		public:
			Cell(int _depth, LLGP::Vector2f _center, LLGP::Vector2f _extents);

			LLGP::Vector2f center;
			LLGP::Vector2f extents;
			std::array<std::unique_ptr<Cell>, 4> children;
			std::vector<LLGP::Collider*> values;
			int depth;

			bool Insert(LLGP::Collider* c, const int& _threshold, const int& _maxDepth);
		private:
			void Split();
			inline bool IsWithinBounds(LLGP::Collider* c);
		};

		int m_Threshold;
		int m_MaxDepth;

		std::unique_ptr<Cell> m_Root;
	};
}
