#include <gtest/gtest.h>
#include <functions.h>

TEST(AVL, assending) {
      AVLTree<int> avl;
      for (int i = 0; i < 10; i++)
            avl.insert(i);
      struct node
      {
            int data;
            int height;
      } sample[10] = {{3, 4},
                      {1, 2},
                      {0, 1},
                      {2, 1},
                      {7, 3},
                      {5, 2},
                      {4, 1},
                      {6, 1},
                      {8, 2},
                      {9, 1}};

      auto list = avl.getListNode();
      list->traverse(
         [](AVLNode<int>*& n, void* v) {
               static int i      = 0;
               node*      sample = (node*) v;
               EXPECT_EQ(n->_data, sample[i].data);
               EXPECT_EQ(n->_height, sample[i].height);
               i++;
         },
         sample);

      delete list;
      list = nullptr;
}