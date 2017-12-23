/*
 * ==========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 2 - Data structures and Algorithms
 * - Fall 2017
 * ==========================================================================================
 */
/**
 * This is done by nguyen anh khoa - 1611617
 *
 * Github repository will be public after online judge closes
 * Github: https://github.com/nganhkhoa/CTDL-ASS2.git
 * Docker:
 * https://cloud.docker.com/swarm/luibo/repository/docker/luibo/ctdl-ass2/general
 *
 * Library use with define so no affect on building online
 *
 * Libraries used:
 *    Spdlog: https://github.com/gabime/spdlog
 *    Googletest: https://github.com/google/googletest
 *
 * This is only in case teacher want to interview me
 */

#include <iostream>
#include <vector>
#include <functional>
#include <math.h>
#include <requestLib.h>
#include <dbLib.h>

#ifdef DEBUGGING
#include <spdlog/spdlog.h>
#endif

using namespace std;

#define GPS_DISTANCE_ERROR 0.005


bool initVMGlobalData(void** pGData) {
      auto vehicleTree = new AVLTree<string>();
      auto rList       = (L1List<VM_Record>*) *pGData;

      rList->traverse(
         [](VM_Record& vmr, void* v) {
               auto    tree = (AVLTree<string>*) v;
               string  id(vmr.id);
               string* ret = nullptr;
               if (tree->find(id, ret))
                     return;

               tree->insert(id);
         },
         vehicleTree);

      *pGData = vehicleTree;

#ifdef DEBUGGING
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("{} vehicles", vehicleTree->getSize());
      file->info("{} vehicles", vehicleTree->getSize());

      auto   list = vehicleTree->getListNode();
      string str;
      list->traverse(
         [](AVLNode<string>*& n, void* v) {
               auto str = (string*) v;
               *str += " " + n->_data;
         },
         &str);

      file->info("vehicles:\n{}", str);
#endif

      return true;
}

void releaseVMGlobalData(void* pGData) {
      auto vehicleTree = (AVLTree<string>*) pGData;
      delete vehicleTree;
      vehicleTree = nullptr;
      pGData      = nullptr;
}


bool print(returnType&, VM_Request&);

bool processRequest(
   VM_Request&        request,
   L1List<VM_Record>& recordList,
   void*              pGData) {
// TODO: Your code goes here
// return false for invlaid events

#ifdef DEBUGGING
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("processing request {}", request.code);
      file->info("processing request {}", request.code);
#endif

      auto vehicleTree = (AVLTree<string>*) pGData;

      returnType r;

      switch (request.code[0] - '0') {
            case 1:
                  r = request1(request, recordList);
                  break;
            case 2:
                  r = request2();
                  break;
            case 3:
                  r = request3();
                  break;
            case 4:
                  r = request4();
                  break;
            case 5:
                  r = request5();
                  break;
            case 6:
                  r = request6();
                  break;
            case 7:
                  r = request7();
                  break;
            case 8:
                  r = request8();
                  break;
            case 9:
                  r = request9();
                  break;
            default:
                  break;
      }

      return print(r, request);
}


returnType request1(VM_Request& request, L1List<VM_Record>& list) {

      if (list.isEmpty())
            return {false};

      auto thisTime = localtime(&list.at(0).timestamp);
      if (thisTime == nullptr)
            return {"cannot get time from data"};

      VM_Record car_1, car_2;
      if (
         sscanf(    // 1_X_Y_hhmmss
            request.code,
            "%1lf_%16[a-zA-Z0-9]_%16[a-zA-Z0-9]_%2d%2d%2d",
            &request.params[0],
            car_1.id,
            car_2.id,
            &thisTime->tm_hour,
            &thisTime->tm_min,
            &thisTime->tm_sec) != 6)
            return {false};

      car_1.timestamp = timegm(thisTime);
      car_2.timestamp = timegm(thisTime);

      vector<VM_Record> record;
      record.push_back(car_1);
      record.push_back(car_2);

      try {
            list.traverse(
               [](VM_Record& r, void* v) {
                     auto record = (vector<VM_Record>*) v;

                     auto& record_1 = record->at(0);
                     if (
                        strcmp(r.id, record_1.id) == 0 &&
                        r.timestamp == record_1.timestamp) {
                           record_1 = r;
                     }

                     auto& record_2 = record->at(1);
                     if (
                        strcmp(r.id, record_2.id) == 0 &&
                        r.timestamp == record_2.timestamp) {
                           record_2 = r;
                     }
               },
               &record);
      } catch (std::exception& e) {
      }

      if (
         record.at(0).longitude == 0 || record.at(0).latitude == 0 ||
         record.at(1).longitude == 0 || record.at(1).latitude == 0)
            return {false};

      auto ret = new L1List<string>();

      char RelativeLat = record.at(0).RelativeLatitudeTo(record.at(1));
      char RelativeLon = record.at(0).RelativeLongitudeTo(record.at(1));

      stringstream ss;
      ss << fixed << setprecision(12) << record.at(0).DistanceTo(record.at(1));

      string distance = ss.str();
      string lat      = to_string(RelativeLat);
      string lon      = to_string(RelativeLon);

      ret->push_back(lon);
      ret->push_back(lat);
      ret->push_back(distance);

      return {ret};
}
returnType request2() {
      return {false};
}
returnType request3() {
      return {false};
}
returnType request4() {
      return {false};
}
returnType request5() {
      return {false};
}
returnType request6() {
      return {false};
}
returnType request7() {
      return {false};
}
returnType request8() {
      return {false};
}
returnType request9() {
      return {false};
}


bool print(returnType& r, VM_Request& req) {
      switch (r.t) {
            case returnType::type::empty:
                  return false;
            case returnType::type::boolean:
                  cout << req.code << ": -1\n";
                  return false;

            case returnType::type::list:
                  cout << req.code << ":";
                  if (r.l->isEmpty())
                        cout << " -1";
                  else
                        r.l->traverse([](string& s) { cout << " " << s; });
                  cout << "\n";
                  delete r.l;
                  r.l = nullptr;
                  return true;

            case returnType::type::tree:
                  return true;

            case returnType::type::floatingpoint:
                  cout << req.code << ": " << fixed << setprecision(12) << r.d
                       << "\n";
                  return true;

            case returnType::type::number:
                  cout << req.code << ": " << r.i << "\n";
                  return true;

            default:
                  // we should not get here
                  return false;
      };
}