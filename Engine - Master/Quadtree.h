#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "MathGeoLib.h"
#include <vector>

class GameObject;

class Quadtree
{
public:
	Quadtree();
	Quadtree(math::AABB limits, int level);
	~Quadtree();

	void Insert(GameObject* go);
	void Intersect(std::vector<GameObject*>& gos, const math::AABB& bbox);
	void Intersect(std::vector<GameObject*>& gos, const math::Frustum& frustum);
	void Intersect(std::vector<GameObject*>& gos, const math::LineSegment& line);
	void Split();

	void Clear();

	void Draw() const;

public:
	AABB QuadTree_Box;

	std::vector<GameObject*> container;

	int maxElements = 0;
	int maxLevels = 0;
	int level = 0;

	std::vector<Quadtree*> children;

};

#endif // __QUADTREE_H__