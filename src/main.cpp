#include <iostream>
#ifndef NDEBUG
#include <debug.h>
#endif
#ifdef UNITTEST
#include <gtest/gtest.h>
#endif

#include <dsaLib.h>
#include <requestLib.h>
#include <dbLib.h>

using namespace std;


/// This function displays the content of database
void display(L1List<VM_Record>& bList) {
      cout.flush();
      bList.traverse(printVMRecord);
}

int main(int narg, char** argv) {
#ifndef NDEBUG
      console->info("Logging system initialized");
#endif    // DEBUG
#ifndef UNITTEST
      L1List<VM_Request> requestList;
      L1List<VM_Record>  db;

      loadRequests(argv[1], requestList);
      loadVMDB(argv[2], db);

      cout << fixed << setprecision(12);    // preset for floating point numbers

      process(requestList, db);

      cout << resetiosflags(ios::showbase) << setprecision(-1);

      return 0;
#else    // UNITTEST
      testing::InitGoogleTest(&argc, argv);
      return RUN_ALL_TESTS();
#endif
}