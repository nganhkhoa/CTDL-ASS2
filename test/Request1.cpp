#include <gtest/gtest.h>
#include <functions.h>


class req1_parsing : public ::testing::Test {
    protected:
      virtual void SetUp() {
            for (auto& s : wrong_req_str)
                  wrong_req.push_back(s);

            VM_Record dummy;
            dummy.timestamp = time(nullptr);
            List.insert(dummy);
      }

      virtual void TearDown() {}


      // empty data set
      AVLTree<VM_Record>       List;
      std::vector<VM_Request>  wrong_req;
      std::vector<std::string> wrong_req_str{"1",
                                             "1_",
                                             "1_1",
                                             "1_1_1",
                                             "1_1_1_1",
                                             "1_1_1_11",
                                             "1_1_1_111",
                                             "1_1_1_1111"};
};

TEST_F(req1_parsing, wrong_code) {
      for (auto& req : wrong_req) {
            auto ret = request1(req, List);
            ASSERT_EQ(ReturnType::type::boolean, ret.t) << req.code;
            EXPECT_FALSE(ret.b);
      }
}


class req1 : public ::testing::Test {
    protected:
      virtual void SetUp() {
            struct tm t;
            t.tm_hour = 11;
            t.tm_min  = 11;
            t.tm_sec  = 11;
            auto tt   = timegm(&t);

            strcpy(rec[0].id, "1");
            rec[0].longitude = 200;
            rec[0].latitude  = 200;
            rec[0].timestamp = tt;

            strcpy(rec[1].id, "2");
            rec[1].longitude = 200;
            rec[1].latitude  = 200;
            rec[1].timestamp = tt;

            list.insert(rec[0]);
            list.insert(rec[1]);
      }
      virtual void TearDown() {}

      AVLTree<VM_Record> list;
      VM_Record          rec[2];
};


TEST_F(req1, not_found) {
      // car not found
      std::string str = "1_0_0_111111";
      VM_Request  req(str);
      auto        ret = request1(req, list);
      ASSERT_EQ(ReturnType::type::number, ret.t);
      EXPECT_EQ(-1, ret.i);

      // first car not found
      str = "1_0_1_111111";
      strcpy(req.code, str.c_str());
      ret = request1(req, list);
      ASSERT_EQ(ReturnType::type::number, ret.t);
      EXPECT_EQ(-1, ret.i);

      // second car not found
      str = "1_1_0_111111";
      strcpy(req.code, str.c_str());
      ret = request1(req, list);
      ASSERT_EQ(ReturnType::type::number, ret.t);
      EXPECT_EQ(-1, ret.i);

      // wrong timestamp
      str = "1_1_0_111110";
      strcpy(req.code, str.c_str());
      ret = request1(req, list);
      ASSERT_EQ(ReturnType::type::number, ret.t);
      EXPECT_EQ(-1, ret.i);
}

TEST_F(req1, correct) {
      std::string str = "1_1_2_111111";
      VM_Request  req(str);
      auto        ret = request1(req, list);
      ASSERT_EQ(ReturnType::type::list, ret.t);
      ASSERT_EQ(3, ret.l->getSize());
}
