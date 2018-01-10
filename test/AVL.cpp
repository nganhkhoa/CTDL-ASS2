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
      } sample[10] = {{0, 1},
                      {1, 2},
                      {2, 1},
                      {3, 4},
                      {4, 1},
                      {5, 2},
                      {6, 1},
                      {7, 3},
                      {8, 2},
                      {9, 1}};

      auto list = avl.getListNode();

      int i = 0;
      for (auto& n : *list) {
            EXPECT_EQ(n->_data, sample[i].data);
            EXPECT_EQ(n->_height, sample[i].height);
            i++;
      }

      delete list;
      list = nullptr;
}


TEST(AVL, dessending) {
      AVLTree<int> avl;
      for (int i = 10; i > 0; i--)
            avl.insert(i);
      struct node
      {
            int data;
            int height;
      } sample[10] = {{1, 1},
                      {2, 2},
                      {3, 3},
                      {4, 1},
                      {5, 2},
                      {6, 1},
                      {7, 4},
                      {8, 1},
                      {9, 2},
                      {10, 1}};

      auto list = avl.getListNode();

      int i = 0;
      for (auto& n : *list) {
            EXPECT_EQ(n->_data, sample[i].data);
            EXPECT_EQ(n->_height, sample[i].height);
            i++;
      }

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
      } sample[15] = {{4, 1},
                      {5, 2},
                      {7, 1},
                      {26, 3},
                      {27, 1},
                      {29, 4},
                      {32, 1},
                      {43, 2},
                      {46, 1},
                      {62, 5},
                      {67, 2},
                      {70, 1},
                      {73, 3},
                      {74, 1},
                      {79, 2}};

      auto list = avl.getListNode();

      int i = 0;
      for (auto& n : *list) {
            EXPECT_EQ(n->_data, sample[i].data);
            EXPECT_EQ(n->_height, sample[i].height);
            i++;
      }

      delete list;
      list = nullptr;
}