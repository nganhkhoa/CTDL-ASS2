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

#ifndef NDEBUG
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


void request1();
void request2();
void request3();
void request4();
void request5();
void request6();
void request7();
void request8();
void request9();


bool processRequest(
   VM_Request&        request,
   L1List<VM_Record>& recordList,
   void*              pGData) {
// TODO: Your code goes here
// return false for invlaid events

#ifndef NDEBUG
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("processing request {}", request.code);
      file->info("processing request {}", request.code);
#endif

      auto vehicleTree = (AVLTree<string>*) pGData;

      switch (request.code[0] - '0') {
            case 1:
                  request1();
            case 2:
                  request2();
            case 3:
                  request3();
            case 4:
                  request4();
            case 5:
                  request5();
            case 6:
                  request6();
            case 7:
                  request7();
            case 8:
                  request8();
            case 9:
                  request9();
            default:
                  break;
      }

      return true;
}


void request1() {}
void request2() {}
void request3() {}
void request4() {}
void request5() {}
void request6() {}
void request7() {}
void request8() {}
void request9() {}