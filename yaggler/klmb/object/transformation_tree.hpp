//
// file : transformation_tree.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/object/transformation_tree.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 13/02/2014 23:12:53
//
//
// Copyright (C) 2014 Timothée Feuillet
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#ifndef __N_10518882341200552082_2101770754__TRANSFORMATION_TREE_HPP__
# define __N_10518882341200552082_2101770754__TRANSFORMATION_TREE_HPP__

#include <deque>
#include <vector>
#include <list>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <tools/memory_pool.hpp>

#include <klmb/object/aabb.hpp>
#include <klmb/object/obb.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      template<typename Node>
      class transformation_tree
      {
        public:
          class node_holder
          {
            private:
              using child_container = std::list<node_holder>;

            public:
              Node *local; // local will be used to _overwrite_ world (if dirty is setted to true, or if the parent has been updated).
              Node *world; // the world transformation.

              child_container &childs;

              node_holder *parent = nullptr;

              // create a child
              node_holder &create_child(const Node &_node = Node())
              {
                Node *local_node_ptr = new (tree->node_pool) Node(_node);
                Node *world_node_ptr = new (tree->node_pool) Node;

                local_node_ptr->compute_matrix();
                local_node_ptr->compute_world(world, world_node_ptr);

                childs_cont.emplace_back(local_node_ptr, world_node_ptr, tree);
                node_holder &ret = childs_cont.back();
                ret.parent = this;
                return ret;
              }

              // remove this node
              void remove()
              {
                if (parent)
                {
                  auto it = std::find(parent->childs.begin(), parent->childs.end(), *this);
                  if (it != parent->childs.end())
                    parent->childs.erase(it);
                  // here the node is deleted.
                }
                else
                  std::cout << "[Y]: trying to remove root" << std::endl;
              }

              // compute and propagate matrices transformations to every childs
              // NOTE: non recursive.
              void recompute_matrices()
              {
                int world_recompute = 0;

                // recompute world if local is dirty
                if (local->dirty)
                {
                  // we recompute world here.
                  world_recompute = 1;

                  local->compute_matrix();
                  if (parent)
                    local->compute_world(parent->world, world);
                  else
                    local->compute_world(nullptr, world);

                  local->dirty = false;
                }
                if (world->dirty)
                {
                  world->compute_matrix();
                  world_recompute = 1;
                }

                std::vector<decltype(childs.begin())> idxs;
                idxs.reserve(100);
                idxs.push_back(childs.begin());

                node_holder *hldr = this;

                bool deinc = false;
                while (idxs.size())
                {
                  deinc = false;

                  auto *current = &idxs.back();

                  while (*current != hldr->childs_cont.end())
                  {
                    bool inc = false;
                    if ((*current)->local->dirty || world_recompute)
                    {
                      // we recompute world here.
                      ++world_recompute;
                      inc = true;

                      (*current)->local->compute_matrix();
                      (*current)->local->compute_world(hldr->world, (*current)->world);

                      (*current)->local->dirty = false;
                      (*current)->world->dirty = false;
                    }
                    // only 'world' has been touched
                    if ((*current)->world->dirty)
                    {
                      (*current)->world->compute_matrix();
                      (*current)->world->dirty = false;

                      ++world_recompute;
                      inc = true;
                    }


                    // iterate over the childs.
                    if ((*current)->childs_cont.size())
                    {
                      hldr = &(**current);
                      ++*current;
                      idxs.push_back(hldr->childs.begin());
                      current = &idxs.back();
                      continue;
                    }

                    if (inc) // no childs. undo incrementation.
                    {
                      deinc = true;
                      --world_recompute;
                    }

                    ++*current;
                  }

                  if (world_recompute && !deinc)
                    --world_recompute;

                  idxs.pop_back();
                  hldr = hldr->parent;
                }
              }

              // ATTENTION: DO NOT USE DIRECTLY.
              node_holder(Node *_local, Node *_world, transformation_tree *_tree) : local(_local), world(_world), childs(childs_cont), tree(_tree)
              {
                // TODO: find another way to do it.
                local->holder = reinterpret_cast<decltype(local->holder)>(this);
                world->holder = reinterpret_cast<decltype(world->holder)>(this);
              }

              ~node_holder()
              {
                if (local)
                {
                  tree->node_pool.destroy(local);
                  tree->node_pool.deallocate(local);
                }
                if (world)
                {
                  tree->node_pool.destroy(world);
                  tree->node_pool.deallocate(world);
                }
              }

              bool operator == (const node_holder &o)
              {
                return local == o.local;
              }

              node_holder &operator = (const node_holder &o)
              {
                if (local)
                {
                  tree->node_pool.destroy(local);
                  tree->node_pool.deallocate(local);
                }
                if (world)
                {
                  tree->node_pool.destroy(world);
                  tree->node_pool.deallocate(world);
                }

                local = new (tree->node_pool) Node(*o.local);
                world = new (tree->node_pool) Node(*o.world);
                local->holder = reinterpret_cast<decltype(local->holder)>(this);
                world->holder = reinterpret_cast<decltype(world->holder)>(this);
                return *this;
              }
              node_holder &operator = (node_holder && o)
              {
                if (local)
                {
                  tree->node_pool.destroy(local);
                  tree->node_pool.deallocate(local);
                }
                if (world)
                {
                  tree->node_pool.destroy(world);
                  tree->node_pool.deallocate(world);
                }

                local = o.local;
                world = o.world;
                o.local = nullptr;
                o.world = nullptr;
                local->holder = reinterpret_cast<decltype(local->holder)>(this);
                world->holder = reinterpret_cast<decltype(world->holder)>(this);
                return *this;
              }

            private:
              node_holder(transformation_tree *_tree) : childs(childs_cont), tree(_tree)
              {
                local = new (tree->node_pool) Node;
                world = new (tree->node_pool) Node;
                local->holder = reinterpret_cast<decltype(local->holder)>(this);
                world->holder = reinterpret_cast<decltype(world->holder)>(this);
              }

            private:
              transformation_tree *tree;
              child_container childs_cont;

              friend class transformation_tree;
              friend child_container;
              friend typename child_container::allocator_type;
          };

        public:
          transformation_tree() : root(this) {}


          cr::memory_pool<Node> node_pool;

          node_holder root;
      };

      namespace transformation_node
      {
        // a node MUST have: compute_matrix(void), compute_world(const node *parent_world, node *world_dest) and dirty

        // the default node
        struct default_node
        {
          transformation_tree<default_node>::node_holder *holder = nullptr;

          glm::vec3 position = glm::vec3(0., 0., 0.);
          glm::vec3 scale = glm::vec3(1., 1., 1.);
          glm::quat rotation = glm::quat();

          bool dirty = false;
          glm::mat4 matrix = glm::mat4(1.); // the local transformation

          aabb *initial_bounding_box = nullptr;// projected for visibility tests ? ;)
                                            // projected for GUI on_clicks ? :)
                                            // (NOTE: not projected here, simply 'world tranformed')

          obb transformed_bounding_box;

          // called when the node is dirty
          // re-compute the matrix,
          // re-init the obb.
          void compute_matrix()
          {
            matrix = glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(scale);

            if (initial_bounding_box)
              transformed_bounding_box.set_transform_from(*initial_bounding_box, position, scale, rotation);
            else
              transformed_bounding_box.set_transform_from(aabb(), position, scale, rotation);
          }

          // called to retransform the world node.
          void compute_world(const default_node *parent_world, default_node *world_dest) const
          {
            if (parent_world)
            {
              world_dest->position = parent_world->position + (parent_world->rotation * position) * parent_world->scale;
              world_dest->scale = parent_world->scale * scale;
              world_dest->rotation = parent_world->rotation * rotation;
            }
            else
            {
              world_dest->position = position;
              world_dest->scale = scale;
              world_dest->rotation = rotation;
            }

            world_dest->initial_bounding_box = initial_bounding_box;
            world_dest->compute_matrix();
            world_dest->dirty = false;
          }

        };
      } // namespace transformation_node

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_10518882341200552082_2101770754__TRANSFORMATION_TREE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

