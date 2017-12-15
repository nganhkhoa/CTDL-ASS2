/*
 * ==========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 2 - Data structures and Algorithms
 * - Fall 2017
 * ==========================================================================================
 */
#include <iostream>
#include <vector>
#include <functional>
#include <math.h>
#include <requestLib.h>
#include <dbLib.h>

#ifndef NDEBUG
#include <spdlog/spdlog.h>
#include <logger.h>
#endif

using namespace std;

#define GPS_DISTANCE_ERROR 0.005


bool initVMGlobalData(void** pGData) {
      // TODO: allocate and initialize global data
      // return false if failed
      return true;
}

void releaseVMGlobalData(void* pGData) {
      // TODO: do your cleanup, left this empty if you don't have any
      // dynamically allocated data
}

bool processRequest(
   VM_Request&        request,
   L1List<VM_Record>& recordList,
   void*              pGData) {
// TODO: Your code goes here
// return false for invlaid events

#ifndef NDEBUG
      auto logger  = logger::get();
      auto console = logger->console();
      auto file    = logger->file();
      console->info("processing request {}", request.code);
      file->info("processing request {}", request.code);
#endif


      return true;
}
