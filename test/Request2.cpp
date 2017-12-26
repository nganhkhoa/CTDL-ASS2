#include <gtest/gtest.h>
#include <functions.h>


class req2_parsing : public ::testing::Test {
    protected:
      virtual void SetUp() {
            for (auto& s : wrong_req_str)
                  wrong_req.push_back(s);


            VM_Record dummy;
            List.insertHead(dummy);
      }
      virtual void TearDown() {}

      std::vector<std::string> wrong_req_str{"2", "2_", "2_"};
      std::vector<VM_Request>  wrong_req;
      L1List<VM_Record>        List;
      L1List<std::string>      vehicles;
};


TEST_F(req2_parsing, wrong_code) {
      for (auto& req : wrong_req) {
            auto ret = request2(req, List, vehicles);
            ASSERT_EQ(returnType::type::boolean, ret.t);
            EXPECT_FALSE(ret.b);
      }
}
