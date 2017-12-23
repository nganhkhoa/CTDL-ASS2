#include <iostream>

#ifdef DEBUGGING
#include <spdlog/spdlog.h>
#endif

#include <dsaLib.h>
#include <requestLib.h>
#include <dbLib.h>

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

/// This function displays the content of database
void display(L1List<VM_Record>& bList) {
      cout.flush();
      bList.traverse(printVMRecord);
}

int main(int narg, char** argv) {
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
      file->info("Logging system initialized");
#endif    // NDEBUG

      L1List<VM_Request> requestList;
      L1List<VM_Record>  db;

#ifdef DEBUGGING
      console->info("Load request from file: {}", argv[1]);
      file->info("Load request from file: {}", argv[1]);
#endif
      loadRequests(argv[1], requestList);

#ifdef DEBUGGING
      console->info("Load data from file: {}", argv[2]);
      file->info("Load data from file: {}", argv[2]);
#endif
      loadVMDB(argv[2], db);

      cout << fixed << setprecision(12);    // preset for floating point numbers

#ifdef DEBUGGING
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

#ifdef DEBUGGING
      console->info("All operations done");
      file->info("All operations done");
      spdlog::drop_all();
#endif
      return 0;
}