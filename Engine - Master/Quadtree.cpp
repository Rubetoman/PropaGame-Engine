#include "Quadtree.h"

#include "GameObject.h"
#include "debugdraw.h"

Quadtree::Quadtree()
{
	QuadTree_Box.SetNegativeInfinity();

	maxElements = 1;
	maxLevels = 3;
	level = 0;
}

Quadtree::Quadtree(AABB limits, int level)
{
	QuadTree_Box = limits;
	maxElements = 1;
	maxLevels = 3;
	this->level = level;
}

Quadtree::~Quadtree()
{
}

void Quadtree::Insert(GameObject* go)
{
	if (go != nullptr && QuadTree_Box.Intersects(go->ComputeBBox()))
	{
		if (children.size() > 0)
		{
			for (auto child : children)
			{
				child->Insert(go);
			}
		}
		else
		{
			container.push_back(go);

			if (level<maxLevels && container.size() > maxElements)
			{
				if (children.size() <= 0)
				{
					Split();
				}

				for (auto cont : container)
				{
					for (auto child : children)
					{
						child->Insert(cont);
					}
				}

				container.clear();
			}
		}
	}
}

void Quadtree::Intersect(std::vector<GameObject*>& gos, const AABB& bbox)
{
	if (QuadTree_Box.Intersects(bbox))
	{
		if (children.size() <= 0)
		{
			for (int i = 0; i < container.size(); i++)
			{
				gos.push_back(container[i]);
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				children[i]->Intersect(gos, bbox);
			}
		}
	}
}

void Quadtree::Split()
{
	/* view from top
 max ________ ________
	|        |        |
	|        |        |
	| first  | second |
	|        |        |
	 -------- --------
	|        |        |
	|        |        |
	| third  | fourth |
	|        |        |
	 -------- --------min*/

	float x, y, z;

	x = QuadTree_Box.MaxX() - QuadTree_Box.MinX();
	y = QuadTree_Box.MaxY() - QuadTree_Box.MinY();
	z = QuadTree_Box.MaxZ() - QuadTree_Box.MinZ();

	math::float3 diagonal = { x / 2,y,z / 2 };

	// 1st Child
	AABB first({ QuadTree_Box.minPoint }, { QuadTree_Box.minPoint + diagonal });
	children.push_back(new Quadtree(first, level + 1));

	// 2nd Child
	math::float3 secondMin = QuadTree_Box.minPoint + math::float3(x / 2, 0.0f, 0.0f);
	AABB second(secondMin, secondMin + diagonal);
	children.push_back(new Quadtree(second, level + 1));

	// 3rd Child
	math::float3 thirdMin = QuadTree_Box.minPoint + math::float3(0.0f, 0.0f, z / 2);
	AABB third(thirdMin, thirdMin + diagonal);
	children.push_back(new Quadtree(third, level + 1));

	// 4th Child
	AABB fourth(QuadTree_Box.maxPoint - diagonal, QuadTree_Box.maxPoint);
	children.push_back(new Quadtree(fourth, level + 1));
}

void Quadtree::Clear()
{
	if (children.size() > 0)
	{
		for (auto child : children)
		{
			child->Clear();
		}

		children.clear();
	}

	for (auto go : container)
	{
		go = nullptr;
	}
	container.clear();
	QuadTree_Box.SetNegativeInfinity();
}

void Quadtree::Draw() const
{
	// Draw children
	for (auto child : children)
	{
		child->Draw();
	}

	dd::aabb(QuadTree_Box.minPoint, QuadTree_Box.maxPoint, math::float3(255, 0, 0), true);
}