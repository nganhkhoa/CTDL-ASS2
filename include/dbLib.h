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
            id[0] = 0;
      }
      VM_Record(const char* busID) {
            strcpy(id, busID);
      }
      // copy constructor
      VM_Record(VM_Record& bus)
          : timestamp(bus.timestamp), longitude(bus.longitude),
            latitude(bus.latitude) {
            strcpy(id, bus.id);
      }

      char RelativeLatitudeTo(double lat) {
            if (latitude - lat >= 0)
                  return 'N';
            else
                  return 'S';
      }
      char RelativeLongitudeTo(double lon) {
            if (longitude - lon >= 0)
                  return 'E';
            else
                  return 'W';
      }
      char RelativeLatitudeTo(const VM_Record& r) {
            return RelativeLatitudeTo(r.latitude);
      }
      char RelativeLongitudeTo(const VM_Record& r) {
            return RelativeLongitudeTo(r.longitude);
      }
      char RelativeLatitudeTo(double lat, double lon) {
            return RelativeLatitudeTo(lat);
      }
      char RelativeLongitudeTo(double lat, double lon) {
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


returnType request1(VM_Request&, L1List<VM_Record>&);
returnType request2();
returnType request3();
returnType request4();
returnType request5();
returnType request6();
returnType request7();
returnType request8();
returnType request9();

#endif    // DSA171A2_DBLIB_H
