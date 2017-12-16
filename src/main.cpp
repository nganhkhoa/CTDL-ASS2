#include <iostream>
#ifdef UNITTEST
#include <gtest/gtest.h>
#endif

#ifndef NDEBUG
#include <spdlog/spdlog.h>
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
            // create two logger for console and file
            auto console = spdlog::stdout_color_mt("console.log");

            // get file name as run time
            time_t      t = time(nullptr);
            std::string file_name(asctime(std::localtime(&t)));
            file_name = file_name.substr(0, 24);
            file_name = "logs/" + file_name + ".log";
            // create log file with name as run time
            auto file = spdlog::basic_logger_mt("file.log", file_name);

      } catch (const spdlog::spdlog_ex& ex) {
            std::cout << "Some error occured " << ex.what() << "\n";
            exit(255);
      }

      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");

      console->set_pattern("[ %H:%M:%S.%F ]\n    >> %v");
      file->set_pattern("[%H:%M:%S.%F]\n    >> %v");
      console->info("Logging system initialized");
      file->info("Logging system initialized");
#endif    // DEBUG
#ifndef UNITTEST
      L1List<VM_Request> requestList;
      L1List<VM_Record>  db;

#ifndef NDEBUG
      console->info("Load request from file: {}", argv[1]);
      file->info("Load request from file: {}", argv[1]);
#endif
      loadRequests(argv[1], requestList);

#ifndef NDEBUG
      console->info("Load data from file: {}", argv[2]);
      file->info("Load data from file: {}", argv[2]);
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
#endif
      return 0;
#else    // UNITTEST
      testing::InitGoogleTest(&narg, argv);
      return RUN_ALL_TESTS();
#endif
}