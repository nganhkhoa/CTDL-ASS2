#include <gtest/gtest.h>
#include <functions.h>


class req1_parsing : public ::testing::Test {
    protected:
      virtual void SetUp() {
            VM_Record dummy;
            dummy.timestamp = time(nullptr);
            List.insertHead(dummy);
      }

      virtual void TearDown() {}


      // empty data set
      L1List<VM_Record>        List;
      std::vector<std::string> wrong_req_str{"1",
                                             "1_",
                                             "1_1",
                                             "1_1_1",
                                             "1_1_1_1",
                                             "1_1_1_11",
                                             "1_1_1_111",
                                             "1_1_1_1111",
                                             "1_1_1_11111"};

      VM_Request wrong_req[9]{
         // init from string
         wrong_req_str[0],
         wrong_req_str[1],
         wrong_req_str[2],
         wrong_req_str[3],
         wrong_req_str[4],
         wrong_req_str[5],
         wrong_req_str[6],
         wrong_req_str[7],
         wrong_req_str[8]
         // one more is complete
      };
};


TEST_F(req1_parsing, wrong_code) {
      for (int i = 0; i < 9; ++i) {
            auto ret = request1(wrong_req[0], List);
            ASSERT_EQ(returnType::type::boolean, ret.t);
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

            list.insertHead(rec[0]);
            list.insertHead(rec[1]);
      }
      virtual void TearDown() {}

      L1List<VM_Record> list;
      VM_Record         rec[2];
};


TEST_F(req1, not_found) {
      // car not found
      std::string str = "1_0_0_111111";
      VM_Request  req(str);
      auto        ret = request1(req, list);
      ASSERT_EQ(returnType::type::boolean, ret.t);
      EXPECT_FALSE(ret.b);

      // first car not found
      str = "1_0_1_111111";
      strcpy(req.code, str.c_str());
      ret = request1(req, list);
      ASSERT_EQ(returnType::type::boolean, ret.t);
      EXPECT_FALSE(ret.b);

      // second car not found
      str = "1_1_0_111111";
      strcpy(req.code, str.c_str());
      ret = request1(req, list);
      ASSERT_EQ(returnType::type::boolean, ret.t);
      EXPECT_FALSE(ret.b);

      // wrong timestamp
      str = "1_1_0_111110";
      strcpy(req.code, str.c_str());
      ret = request1(req, list);
      ASSERT_EQ(returnType::type::boolean, ret.t);
      EXPECT_FALSE(ret.b);
}

TEST_F(req1, correct) {
      std::string str = "1_1_2_111111";
      VM_Request  req(str);
      auto        ret = request1(req, list);
      ASSERT_EQ(returnType::type::list, ret.t);
      ASSERT_EQ(3, ret.l->getSize());

      string lhs, rhs;
      lhs = to_string(rec[0].RelativeLongitudeTo(rec[1]));
      rhs = ret.l->at(0);
      EXPECT_EQ(lhs, rhs);

      lhs = to_string(rec[0].RelativeLatitudeTo(rec[1]));
      rhs = ret.l->at(1);
      EXPECT_EQ(lhs, rhs);

      std::stringstream ss;
      ss << fixed << setprecision(12) << rec[0].DistanceTo(rec[1]);
      lhs = ss.str();
      rhs = ret.l->at(2);
      EXPECT_EQ(lhs, rhs);
}