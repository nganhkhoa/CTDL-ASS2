#include <gtest/gtest.h>
#include <functions.h>

TEST(LinkedList, ascending) {
      L1List<int> list;

      for (int i = 0; i < 10; ++i)
            list.insertHead(i);

      int i = 9;
      for (auto& x : list)
            EXPECT_EQ(x, i--);
}