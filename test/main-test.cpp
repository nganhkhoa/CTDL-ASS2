#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

#ifdef DEBUGGING
#include <spdlog/spdlog.h>
#endif

#include <gtest/gtest.h>

// to be include here

// use these tags to remind others
// Tags: TODO:, BUGME:, NEW:, MORETEST:

using namespace std;

#ifdef DEBUGGING

void initLoggers() {
      // setup loggers
      try {
            // create two logger for console and file
            auto console = spdlog::stdout_color_mt("console.log");

            // get file name as run time
            time_t     t  = time(nullptr);
            struct tm* lt = std::localtime(&t);
            string     file_name =
               to_string(lt->tm_year + 1900) + "-" + to_string(lt->tm_mon + 1) +
               "-" + to_string(lt->tm_mday) + " " + to_string(lt->tm_hour) +
               ":" + to_string(lt->tm_min) + ":" + to_string(lt->tm_sec);
            file_name = "logs/" + file_name + ".log";
            // create log file with name as run time
            auto file = spdlog::basic_logger_mt("file.log", file_name);

            console->set_pattern("[ %H:%M:%S.%F ]\n    >> %v");
            file->set_pattern("[%H:%M:%S.%F]\n    >> %v");

      } catch (const spdlog::spdlog_ex& ex) {
            std::cout << "Some error occured " << ex.what() << "\n";
            exit(255);
      }
}

#endif    // NDEBUG

int main(int argc, char** argv, char** envp) {
#ifdef _WIN32
      system("cls");
#elif __unix
      system("clear");
#endif

#ifdef DEBUGGING
      initLoggers();
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("Logging system initialized");
      console->info("Running tests");
      file->info("Logging system initialized");
      file->info("Running tests");
#endif    // NDEBUG

      testing::InitGoogleTest(&argc, argv);
      int ret = RUN_ALL_TESTS();
      return ret;
}