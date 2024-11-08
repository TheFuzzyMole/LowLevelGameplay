#include <Core/Components/BoxCollider.h>
#include <Core/Components/CircleCollider.h>
#include <Core/QuadTree.h>
#include <iostream>

namespace LLGP
{
	QuadTree::QuadTree(int _threshold, int _maxDepth, LLGP::Vector2f& min, LLGP::Vector2f& max)
	{
		m_Threshold = _threshold;
		m_MaxDepth = _maxDepth;
		m_Root = std::make_unique<Cell>(0, (max + min) / 2, (max - min) / 2);
	}

	void QuadTree::Insert(LLGP::Collider* c)
	{
		if (!m_Root->Insert(c, m_Threshold, m_MaxDepth)) { std::cout << "trying to insert collider into quadtree but the collider is larger than the root object!" << std::endl; }
	}

	bool QuadTree::Query(const LLGP::Vector2f& _center, const LLGP::Vector2f& _extents, std::vector<LLGP::Collider*>& outColliders)
	{
		return m_Root->Query(_center, _extents, outColliders);
	}

	QuadTree::Cell::Cell(int _depth, LLGP::Vector2f _center, LLGP::Vector2f _extents)
		: depth(_depth), center(_center), extents(_extents) {}

	bool QuadTree::Cell::Insert(LLGP::Collider* c, const int& _threshold, const int& _maxDepth)
	{
		if (!IsWithinBounds(c)) { return false; }

		if (children[0])
		{
			bool inserted = false;
			for (int childIdx = 0; childIdx < 4; childIdx++)
			{
				if (children[childIdx]->Insert(c, _threshold, _maxDepth)) { inserted = true; break; }
			}
			if (!inserted) { values.push_back(c); }
		}
		else if (depth < _maxDepth)
		{
			values.push_back(c);
			if (values.size() > _threshold)
			{
				Split();
				std::vector<LLGP::Collider*> toRedistribute = values;
				values.clear();
				for (int Idx = 0; Idx < toRedistribute.size(); Idx++)
				{
					Insert(toRedistribute[Idx], _threshold, _maxDepth);
				}
			}
		}
		else
		{
			values.push_back(c);
		}
		return true;
	}

	bool QuadTree::Cell::Query(const LLGP::Vector2f& _center, const LLGP::Vector2f& _extents, std::vector<LLGP::Collider*>& outColliders)
	{
		LLGP::Vector2f qMin, qMax, cMin, cMax;
		qMin = _center - _extents;
		qMax = _center + _extents;
		cMin = center - extents;
		cMax = center + extents;

		if (qMin.x >= cMax.x || qMax.x <= cMin.x || qMin.y >= cMax.y || qMax.y <= cMin.y) { return false; }

		if (qMin.x <= cMin.x && qMin.y <= cMin.y && qMax.x >= cMax.x && qMax.y >= cMax.y) { RecursiveFill(outColliders); return true; }

		for (LLGP::Collider* c : values)
		{
			cMin = c->GetCenter() - c->GetBoundsExtents();
			cMax = c->GetCenter() + c->GetBoundsExtents();
			if (qMin.x >= cMax.x || qMax.x <= cMin.x || qMin.y >= cMax.y || qMax.y <= cMin.y) { continue; }
			outColliders.push_back(c);
		}

		if (children[0])
		{
			for (int childIdx = 0; childIdx < 4; childIdx++)
			{
				children[childIdx]->Query(_center, _extents, outColliders);
			}
		}

		return true;
	}

	void QuadTree::Cell::RecursiveFill(std::vector<LLGP::Collider*>& outColliders)
	{
		for (LLGP::Collider* c : values)
		{
			outColliders.push_back(c);
		}

		if (children[0])
		{
			for (int childIdx = 0; childIdx < 4; childIdx++)
			{
				children[childIdx]->RecursiveFill(outColliders);
			}
		}
	}

	void QuadTree::Cell::Split()
	{
		LLGP::Vector2f newExtents = extents / 2;
		children[0] = std::make_unique<Cell>(depth + 1, LLGP::Vector2f(center.x - newExtents.x, center.y - newExtents.y), newExtents);
		children[1] = std::make_unique<Cell>(depth + 1, LLGP::Vector2f(center.x + newExtents.x, center.y - newExtents.y), newExtents);
		children[2] = std::make_unique<Cell>(depth + 1, LLGP::Vector2f(center.x - newExtents.x, center.y + newExtents.y), newExtents);
		children[3] = std::make_unique<Cell>(depth + 1, LLGP::Vector2f(center.x + newExtents.x, center.y + newExtents.y), newExtents);
	}

	bool QuadTree::Cell::IsWithinBounds(LLGP::Collider* c)
	{
		return (c->GetPosition() - center + c->GetBoundsExtents()).GetSqrMagnitude() < extents.GetSqrMagnitude();
	}
}
