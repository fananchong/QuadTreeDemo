#include "stdafx.h"
#include "QuadTreeRender.h"


QuadTreeRender::QuadTreeRender(math::quad_tree<Item>* tree)
	:tree_(tree)
{

}

QuadTreeRender::~QuadTreeRender()
{
}

void QuadTreeRender::Render(Graphics* graphics)
{
	tree_->foreach([this, graphics](math::quad_tree<Item>::quad_tree_node_ptr_t node) {
		if (!node->contents().empty())
		{
			for (auto& item : node->contents())
			{
				SolidBrush sb(Color::Blue);
				graphics->FillEllipse(&sb, ToGDIRect(item.rect()));
				//graphics->FillEllipse(&sb, Rect(item.rect_.origin.x, tree_->rect().size.height - item.rect_.origin.y, item.rect_.size.width, item.rect_.size.height));
			}
		}

		Rect rct = ToGDIRect(node->bounds());

		{
			Pen pen(Color::Black,2.0f);

			graphics->DrawRectangle(&pen, rct);

		}

		//Pen pen(Color::Blue);
		//rct.Inflate(-1, -1);
		//graphics->DrawRectangle(&pen, rct);
	});

}
