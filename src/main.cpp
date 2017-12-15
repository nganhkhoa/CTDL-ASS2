#include <iostream>
#ifndef NDEBUG
#include <spdlog/spdlog.h>
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
      // setup loggers
      try {
            // set async mode
            spdlog::set_async_mode(8192);
            // create two logger for console and file
            auto console = spdlog::stdout_color_mt("console.log");
            auto file    = spdlog::rotating_logger_mt(
               "file.log", "logs/log.txt", 1024 * 1024 * 5, 3);

            spdlog::drop_all();
      } catch (const spdlog::spdlog_ex& ex) {
            cout << "Some error occured\n";
            return 255;
      }
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("Logging system initialized");
      file->info("Logging system initialized");
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
      console->info("Begin processing events");
      file->info("Begin processing events");
#endif
      process(requestList, db);

      cout << resetiosflags(ios::showbase) << setprecision(-1);

#ifndef NDEBUG
      console->info("All operation done");
      file->info("All operation done");
#endif
      return 0;
#else    // UNITTEST
      testing::InitGoogleTest(&argc, argv);
      return RUN_ALL_TESTS();
#endif
}