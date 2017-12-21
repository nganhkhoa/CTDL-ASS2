#include <gtest/gtest.h>
#include <functions.h>


class req1 : public ::testing::Test {
    protected:
      virtual void SetUp() {}

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


TEST_F(req1, wrong_code) {
      for (int i = 0; i < 10; ++i) {
            auto ret = request1(wrong_req[0], List);
            ASSERT_EQ(returnType::type::boolean, ret.t);
            EXPECT_FALSE(ret.b);
      }
}