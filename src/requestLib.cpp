/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa
 * University Description : library for Assignment 2 - Data structures and
 * Algorithms - Fall 2017 This library contains functions used for event
 * management
 * =========================================================================================
 */
/**
 * This is done by nguyen anh khoa - 1611617
 *
 * Github repository will be public after online judge closes
 * Github: https://github.com/nganhkhoa/CTDL-ASS2.git
 * Docker:
 * https://cloud.docker.com/swarm/luibo/repository/docker/luibo/ctdl-ass2/general
 *
 * Libraries are used with define so no affect on building online
 *
 * Libraries used:
 *    Spdlog: https://github.com/gabime/spdlog
 *    Googletest: https://github.com/google/googletest
 *
 * This is only in case teacher want to interview me
 */

#include <requestLib.h>

#ifdef DEBUGGING
#include <spdlog/spdlog.h>
#endif

void loadRequests(char* fName, L1List<VM_Request>& rList) {
      ifstream inFile(fName);

      if (!inFile) {
            cout << "The file is not found!";
            return;
      }

      string line;

      while (getline(inFile, line)) {
            if (line[line.length() - 1] == ';')
                  line.erase(line.length() - 1);

            if (line.empty())
                  continue;

            istringstream iss(line);
            while (getline(iss, line, ' ')) {
                  VM_Request req(line);
                  rList.insertHead(req);
            }
      }

      rList.reverse();
}
