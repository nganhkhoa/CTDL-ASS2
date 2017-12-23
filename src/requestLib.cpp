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
      bool   end;
      while (getline(inFile, line, ' ')) {
            if (line[line.length() - 1] == ';') {
                  line.erase(line.length() - 1);
                  end = true;
            }

            VM_Request req(line);
            rList.insertHead(req);

            if (end)
                  break;
      }
      rList.reverse();
}
