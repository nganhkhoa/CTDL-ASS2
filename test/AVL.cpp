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


TEST(AVL, dessending) {
      AVLTree<int> avl;
      for (int i = 10; i < 0; i--)
            avl.insert(i);
      struct node
      {
            int data;
            int height;
      } sample[10] = {{7, 4},
                      {3, 3},
                      {2, 2},
                      {1, 1},
                      {5, 2},
                      {4, 1},
                      {6, 1},
                      {9, 2},
                      {8, 1},
                      {10, 1}};

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


TEST(AVL, random1) {
      AVLTree<int> avl;
      int arr[15] = {32, 73, 62, 29, 79, 26, 67, 70, 43, 27, 4, 46, 7, 74, 5};
      for (int i = 0; i < 15; i++)
            avl.insert(arr[i]);

      struct node
      {
            int data;
            int height;
      } sample[15] = {{62, 5},
                      {29, 4},
                      {26, 3},
                      {5, 2},
                      {4, 1},
                      {7, 1},
                      {27, 1},
                      {43, 2},
                      {32, 1},
                      {46, 1},
                      {73, 3},
                      {67, 2},
                      {70, 1},
                      {79, 2},
                      {74, 1}};

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