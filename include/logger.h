#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <spdlog/spdlog.h>


class logger {
      static logger*                  _logger;
      std::shared_ptr<spdlog::logger> _console;
      std::shared_ptr<spdlog::logger> _file;

      logger() {
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

            _console = spdlog::get("console.log");
            _file    = spdlog::get("file.log");
            _console->info("Logging system initialized");
            _file->info("Logging system initialized");
      }

    public:
      static void init() {
            _logger = new logger();
      }

      static logger* get() {
            return _logger;
      }

      std::shared_ptr<spdlog::logger> console() {
            return _console;
      }

      std::shared_ptr<spdlog::logger> file() {
            return _file;
      }

      ~logger() {
            spdlog::drop_all();
      }
};

#endif