#include <iostream>
#ifdef UNITTEST
#include <gtest/gtest.h>
#endif

#ifndef NDEBUG
#include <spdlog/spdlog.h>
#include <logger.h>
logger* logger::_logger = nullptr;
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
      logger::init();
      auto logger  = logger::get();
      auto console = logger->console();
      auto file    = logger->file();
#endif    // DEBUG
#ifndef UNITTEST
      L1List<VM_Request> requestList;
      L1List<VM_Record>  db;

#ifndef NDEBUG
      console->info("Load request");
      file->info("Load request");
#endif
      loadRequests(argv[1], requestList);

#ifndef NDEBUG
      console->info("Load data");
      file->info("Load data");
#endif
      loadVMDB(argv[2], db);

      cout << fixed << setprecision(12);    // preset for floating point numbers

#ifndef NDEBUG
      console->info(
         "Begin processing {} events with {} records",
         requestList.getSize(),
         db.getSize());
      file->info(
         "Begin processing {} events with {} records",
         requestList.getSize(),
         db.getSize());
#endif
      process(requestList, db);

      cout << resetiosflags(ios::showbase) << setprecision(-1);

#ifndef NDEBUG
      console->info("All operations done");
      file->info("All operations done");
      delete logger;
#endif
      return 0;
#else    // UNITTEST
      testing::InitGoogleTest(&narg, argv);
      return RUN_ALL_TESTS();
#endif
}