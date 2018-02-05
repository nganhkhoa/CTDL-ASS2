/*
 * =========================================================================================
 * Name        : dbLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa
 * University Description : library for Assignment 2 - Data structures and
 * Algorithms - Fall 2017 This library contains functions used for database
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

#ifndef DSA171A2_DBLIB_H
#define DSA171A2_DBLIB_H

#include <string>
#include <string.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <functional>

#include <dsaLib.h>
#include <requestLib.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define ID_MAX_LENGTH 16

struct VM_Record
{
      char   id[ID_MAX_LENGTH];
      time_t timestamp;
      double longitude, latitude;

      // default constructor
      VM_Record() {
            id[0]     = 0;
            timestamp = 0;
            longitude = 0;
            latitude  = 0;
      }
      VM_Record(const char* busID) {
            strcpy(id, busID);
      }
      // copy constructor
      VM_Record(const VM_Record& bus)
          : timestamp(bus.timestamp), longitude(bus.longitude),
            latitude(bus.latitude) {
            strcpy(id, bus.id);
      }

      bool operator<(const VM_Record& r) {
            if (timestamp == r.timestamp)
                  return strcmp(id, r.id) < 0;
            else
                  return timestamp < r.timestamp;
      }

      bool operator==(const VM_Record& r) {
            return strcmp(id, r.id) == 0 && timestamp == r.timestamp;
      }

      std::string RelativeLatitudeTo(double lat) {
            if (latitude - lat >= 0)
                  return "N";
            else
                  return "S";
      }
      std::string RelativeLongitudeTo(double lon) {
            if (longitude - lon >= 0)
                  return "E";
            else
                  return "W";
      }
      std::string RelativeLatitudeTo(const VM_Record& r) {
            return RelativeLatitudeTo(r.latitude);
      }
      std::string RelativeLongitudeTo(const VM_Record& r) {
            return RelativeLongitudeTo(r.longitude);
      }
      std::string RelativeLatitudeTo(double lat, double lon) {
            return RelativeLatitudeTo(lat);
      }
      std::string RelativeLongitudeTo(double lat, double lon) {
            return RelativeLongitudeTo(lon);
      }

      double DistanceTo(double lat, double lon);
      double DistanceTo(const VM_Record& r);

      bool isInRange(double lat, double lon, double radius) {
            return DistanceTo(lat, lon) <= radius;
      }
      bool isInRange(const VM_Record& r, double radius) {
            return DistanceTo(r) <= radius;
      }
};

void   printVMRecord(VM_Record&);
void   strPrintTime(char* des, time_t& t);
bool   parseVMRecord(char*, VM_Record&);
void   loadVMDB(char*, L1List<VM_Record>&);
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);

// from processData.cpp
bool processRequest(VM_Request&, L1List<VM_Record>&, void*);

/// NOTE: student may create this function to allocate some global data
bool initVMGlobalData(void** pGData);
/// NOTE: student must defined this function if they use dynamically allocated
/// global data. If student do not use any dynamic global data, please define
/// this function as empty function in your code (file processData.cpp) as
/// follows void releaseBusGlobalData() {}
void releaseVMGlobalData(void* pGData);

void process(L1List<VM_Request>& requestList, L1List<VM_Record>& recordList);


ReturnType* request1(VM_Request&, AVLTree<VM_Record>&);
ReturnType* request2(VM_Request&, AVLTree<VM_Record>&, const size_t&);
ReturnType* request3(VM_Request&, AVLTree<VM_Record>&, const size_t&);
ReturnType* request4(VM_Request&, AVLTree<VM_Record>&, const size_t&);
ReturnType* request5(VM_Request&, AVLTree<VM_Record>&, AVLTree<string>&);
ReturnType* request6(VM_Request&, AVLTree<VM_Record>&);
ReturnType* request7(VM_Request&, AVLTree<VM_Record>&);
ReturnType* request8(VM_Request&, AVLTree<VM_Record>&, AVLTree<string>&);
ReturnType* request9(VM_Request&, AVLTree<VM_Record>&, AVLTree<string>&);

#endif    // DSA171A2_DBLIB_H
