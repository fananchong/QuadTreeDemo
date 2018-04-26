#pragma once
#include <array>
#include <vector>
#include <memory>
#include <stack>
#include "rect.hpp"

namespace math
{
	template<typename TItem>
	class quad_tree_node:public std::enable_shared_from_this<quad_tree_node<TItem>>
	{
	public:
		using item_t = TItem;
		using quad_tree_node_t = quad_tree_node<item_t>;
		using quad_tree_node_ptr_t = std::shared_ptr<quad_tree_node_t>;

		// Construct a quadtree node with the given bounds 
		explicit quad_tree_node(const math::rect& bounds)
			:bounds_(bounds)
		{
		}

		bool empty() const
		{
			return (bounds_.empty() || nodes_.size() == 0);
		}

		const auto& contents()
		{
			return contents_;
		}

		const math::rect& bounds() const
		{
			return bounds_;
		}

		size_t count() const
		{
			size_t n = 0;
			for (auto& node : nodes_)
			{
				n += node->count();
			}

			n += contents_.size();
			return n;
		}

		void subtree_contents(std::vector<item_t>& ret)
		{
			for (auto& node : nodes_)
			{
				node->subtree_contents(ret);
			}

			for (auto& item : contents_)
			{
				ret.push_back(item);
			}
		}

		void query(const math::rect& queryArea, std::vector<item_t>& ret)
		{
			for (auto& item : contents_)
			{
				if (queryArea.intersects(item.rect()))
				{
					ret.push_back(item);
				}
			}

			for (auto& node : nodes_)
			{
				if (node->empty())
				{
					continue;
				}

				// Case 1: search area completely contained by sub-quad
				// if a node completely contains the query area, go down that branch
				// and skip the remaining nodes (break this loop)
				if (node->bounds().contains(queryArea))
				{
					node->query(queryArea,ret);
					break;
				}

				// Case 2: Sub-quad completely contained by search area 
				// if the query area completely contains a sub-quad,
				// just add all the contents of that quad and it's children 
				// to the result set. You need to continue the loop to test 
				// the other quads
				if (queryArea.contains(node->bounds()))
				{
					node->subtree_contents(ret);
					continue;
				}

				// Case 3: search area intersects with sub-quad
				// traverse into this quad, continue the loop to search other
				// quads

				if (node->bounds().intersects(queryArea))
				{
					node->query(queryArea, ret);
				}
			}
		}

		bool insert(const item_t& item)
		{
			// if the item is not contained in this quad, there's a problem
			if (!bounds_.contains(item.rect()))
			{
				//assert(false && "feature is out of the bounds of this quadtree node");
				return false;
			}

			// if the subnodes are null create them. may not be sucessfull: see below
			// we may be at the smallest allowed size in which case the subnodes will not be created
			if (nodes_.size() == 0)
			{
				create_sub_nodes();
			}

			// for each subnode:
			// if the node contains the item, add the item to that node and return
			// this recurses into the node that is just large enough to fit this item
			for (auto& node : nodes_)
			{
				if(node->bounds().contains(item.rect()))
				{
					return node->insert(item);		
				}
			}

			// if we make it to here, either
			// 1) none of the subnodes completely contained the item. or
			// 2) we're at the smallest subnode size allowed 
			// add the item to this node's contents.
			contents_.push_back(item);
			return true;
		}

		template<typename Action>
		void foreach(Action&& f)
		{
			f(shared_from_this());

			for (auto& node : nodes_)
			{
				node->foreach(std::forward<Action>(f));
			}
		}

		void clear()
		{
			contents_.clear();
			for (auto& node : nodes_)
			{
				node->clear();
			}
		}

	private:
		void create_sub_nodes()
		{
			// the smallest subnode has an area 
			if ((bounds_.width*bounds_.height) <= 10)
				return;

			float halfw = bounds_.width / 2.f;
			float halfh = bounds_.height / 2.f;

			// left top node
			nodes_.push_back(std::make_shared<quad_tree_node_t>(math::rect(bounds_.x, bounds_.midy(), halfw, halfh)));
			// left bottom node
			nodes_.push_back(std::make_shared<quad_tree_node_t>(math::rect(bounds_.x, bounds_.y, halfw, halfh)));
			// right top node
			nodes_.push_back(std::make_shared<quad_tree_node_t>(math::rect(bounds_.midx(), bounds_.midy(), halfw, halfh)));
			// right bottom node
			nodes_.push_back(std::make_shared<quad_tree_node_t>(math::rect(bounds_.midx(), bounds_.y, halfw, halfh)));
		}

	private:
		// The area of this node
		math::rect bounds_;
		std::vector<item_t> contents_;
		// The child nodes of the QuadTree
		std::vector<quad_tree_node_ptr_t> nodes_;
	};

	template<typename TItem>
	class quad_tree
	{
	public:
		using item_t = TItem;
		using quad_tree_node_t = quad_tree_node<item_t>;
		using quad_tree_node_ptr_t = std::shared_ptr<quad_tree_node_t>;

		explicit quad_tree(const math::rect& rct)
			:rect_(rct)
		{
			root_ = std::make_shared<quad_tree_node_t>(rect_);
		}

		size_t count()
		{
			return root_->count();
		}

		template<typename... Args>
		bool insert(Args... args)
		{
			return root_->insert(item_t(std::forward<Args>(args)...));
		}

		bool insert(const item_t& e)
		{
			return root_->insert(e);
		}

		auto query(const math::rect& rct)
		{
			std::vector<item_t> ret;
			root_->query(rct, ret);
			return ret;
		}

		template<typename Action>
		void foreach(Action&& f)
		{
			root_->foreach(std::forward<Action>(f));
		}

		const math::rect& rect() const
		{
			return rect_;
		}

		void clear()
		{
			root_->clear();
		}

	private:
		const math::rect rect_;
		quad_tree_node_ptr_t root_;
	};
}