#include <gtest/gtest.h>
#include <functions.h>


class req3_parsing : public ::testing::Test {
    protected:
      virtual void SetUp() {
            for (auto& s : wrong_req_str)
                  wrong_req.push_back(s);


            VM_Record dummy;
            List.insert(dummy);
      }
      virtual void TearDown() {}

      std::vector<std::string> wrong_req_str{"3", "3_", "3_"};
      std::vector<VM_Request>  wrong_req;
      AVLTree<VM_Record>       List;
};


TEST_F(req3_parsing, wrong_code) {
      for (auto& req : wrong_req) {
            auto ret = request2(req, List, 0);
            ASSERT_EQ(returnType::type::boolean, ret.t);
            EXPECT_FALSE(ret.b);
      }
}
