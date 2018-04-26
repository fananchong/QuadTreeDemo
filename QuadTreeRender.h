#pragma once
#include <unordered_map>

#include "quad_tree.hpp"

#include <GdiPlus.h>
using namespace Gdiplus;

struct Item
{
	Item(float x_, float y_, float size_)
		:x(x_),y(y_),size(size_)
	{
	}

	Item(const Item&) = default;
	Item& operator =(const Item&) = default;

	math::rect rect() const
	{
		return math::rect(x-size/2.0f,y-size/2.0f, size, size) ;
	}

	float x;
	float y;
	float size;
};

class QuadTreeRender
{
public:
	QuadTreeRender(math::quad_tree<Item>*);
	~QuadTreeRender();

	void Render(Graphics* graphics);

	static Rect ToGDIRect(const math::rect& rect)
	{
		return Rect(Point(rect.x, rect.y),Size(rect.width, rect.height));
	}

	static math::rect ToMathRect(float height, const Rect& rect)
	{
		return math::rect(rect.X, height - rect.Y, rect.Width, rect.Height);
	}

private:
	math::quad_tree<Item>* tree_;
};

