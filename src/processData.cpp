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

#if !defined NDEBUG && defined CMAKED
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

#if !defined NDEBUG && defined CMAKED
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


bool processRequest(
   VM_Request&        request,
   L1List<VM_Record>& recordList,
   void*              pGData) {
// TODO: Your code goes here
// return false for invlaid events

#if !defined NDEBUG && defined CMAKED
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("processing request {}", request.code);
      file->info("processing request {}", request.code);
#endif

      auto vehicleTree = (AVLTree<string>*) pGData;

      switch (request.code[0] - '0') {
            case 1:
                  request1(request, recordList);
                  break;
            case 2:
                  request2();
                  break;
            case 3:
                  request3();
                  break;
            case 4:
                  request4();
                  break;
            case 5:
                  request5();
                  break;
            case 6:
                  request6();
                  break;
            case 7:
                  request7();
                  break;
            case 8:
                  request8();
                  break;
            case 9:
                  request9();
                  break;
            default:
                  break;
      }

      return true;
}


returnType request1(VM_Request& request, L1List<VM_Record>& list) {

      struct tm thisTime;
      char      car_1[ID_MAX_LENGTH];
      char      car_2[ID_MAX_LENGTH];
      if (
         sscanf(    // 1_X_Y_hhmmss
            request.code,
            "%1lf_%16[a-zA-Z0-9]_%16[a-zA-Z0-9]_%2d%2d%2d",
            &request.params[0],
            car_1,
            car_2,
            &thisTime.tm_hour,
            &thisTime.tm_min,
            &thisTime.tm_sec) != 6)
            return {false};

      auto ret = new L1List<void*>();


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