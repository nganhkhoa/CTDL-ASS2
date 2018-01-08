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
 * Libraries are used with define so no affect on building online
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
      auto vehicles = new L1List<string>();
      auto rList    = (L1List<VM_Record>*) *pGData;

      rList->traverse(
         [](VM_Record& vmr, void* v) {
               auto   vehicles = (L1List<string>*) v;
               string id(vmr.id);
               if (vehicles->find(id) != nullptr)
                     vehicles->insertHead(id);
         },
         vehicles);

      *pGData = vehicles;

#ifdef DEBUGGING
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("{} vehicles", vehicles->getSize());
      file->info("{} vehicles", vehicles->getSize());

      string str;
      vehicles->traverse(
         [](string& s, void* v) {
               auto str = (string*) v;
               *str += " " + s;
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

      auto vehicles = (L1List<string>*) pGData;

      returnType r;

      switch (request.code[0] - '0') {
            case 1:
                  r = request1(request, recordList);
                  break;
            case 2:
                  r = request2(request, recordList, *vehicles);
                  break;
            case 3:
                  r = request3(request, recordList, *vehicles);
                  break;
            case 4:
                  r = request4(request, recordList, *vehicles);
                  break;
            case 5:
                  r = request5(request, recordList);
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

      auto thisTime = gmtime(&list.at(0).timestamp);
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
                           strcpy(record_1.id, "");
                     }

                     auto& record_2 = record->at(1);
                     if (
                        strcmp(r.id, record_2.id) == 0 &&
                        r.timestamp == record_2.timestamp) {
                           record_2 = r;
                           strcpy(record_2.id, "");
                     }

                     if (
                        strcmp(record_1.id, "") == 0 &&
                        strcmp(record_2.id, "") == 0)
                           throw true;
               },
               &record);
      } catch (std::exception& e) {
            // dummy exception
      } catch (bool& b) {
            // if both car is found
            auto ret = new L1List<string>();

            char RelativeLat = record.at(0).RelativeLatitudeTo(record.at(1));
            char RelativeLon = record.at(0).RelativeLongitudeTo(record.at(1));

            stringstream ss;
            ss << fixed << setprecision(12)
               << record.at(0).DistanceTo(record.at(1));

            string distance = ss.str();
            string lat      = to_string(RelativeLat);
            string lon      = to_string(RelativeLon);

            ret->push_back(lon);
            ret->push_back(lat);
            ret->push_back(distance);

            return {ret};
      }

      // when one car is not found
      return {false};
}
returnType request2(
   VM_Request&        req,
   L1List<VM_Record>& list,
   L1List<string>&    vehicles) {
      if (list.isEmpty())
            return {false};

      char direction;
      if (
         sscanf(
            req.code,
            "%1lf_%10lf_%1[EW]",
            &req.params[0],
            &req.params[1],
            &direction) != 3)
            return {false};

      struct Ans
      {
            AVLTree<string> out;
            double          longitude;
            char            direction;
            size_t          num_vehicle;
      };

      Ans ans;
      ans.longitude   = req.params[1];
      ans.direction   = direction;
      ans.num_vehicle = vehicles.getSize();

      try {
            list.traverse(
               [](VM_Record& vmr, void* v) {
                     Ans* ans = (Ans*) v;
                     if (
                        vmr.RelativeLongitudeTo(ans->longitude) !=
                        ans->direction)
                           return;
                     string id(vmr.id);

                     ans->out.insert(
                        id, [](string& s1, string& s2) { return s1 == s2; });

                     if (ans->num_vehicle == ans->out.getSize()) {
                           throw true;
                     }
               },
               &ans);
      } catch (bool& b) {
            // every vehicles went off track
            return {(int) 0};
      }

      return {(int) (vehicles.getSize() - ans.out.getSize())};
}
returnType request3(
   VM_Request&        req,
   L1List<VM_Record>& list,
   L1List<string>&    vehicles) {
      if (list.isEmpty())
            return {false};

      char direction;
      if (
         sscanf(
            req.code,
            "%1lf_%10lf_%1[NS]",
            &req.params[0],
            &req.params[1],
            &direction) != 3)
            return {false};

      struct Ans
      {
            AVLTree<string> out;
            double          latitude;
            char            direction;
            size_t          num_vehicle;
      };

      Ans ans;
      ans.latitude    = req.params[1];
      ans.direction   = direction;
      ans.num_vehicle = vehicles.getSize();

      try {
            list.traverse(
               [](VM_Record& vmr, void* v) {
                     Ans* ans = (Ans*) v;
                     if (
                        vmr.RelativeLatitudeTo(ans->latitude) != ans->direction)
                           return;

                     string id(vmr.id);
                     ans->out.insert(
                        id, [](string& s1, string& s2) { return s1 == s2; });

                     if (ans->num_vehicle == ans->out.getSize())
                           throw true;
               },
               &ans);
      } catch (bool& b) {
            return {(int) 0};
      }

      return {(int) (vehicles.getSize() - ans.out.getSize())};
}
returnType request4(
   VM_Request&        req,
   L1List<VM_Record>& list,
   L1List<string>&    vehicles) {
      if (list.isEmpty())
            return {(int) 0};

      if (
         sscanf(
            req.code,
            "%1lf_%10lf_%10lf_%10lf_%10lf_%10lf",
            &req.params[0],    // 1 number
            &req.params[1],    // atmost 10 Along with sign
            &req.params[2],    // atmost 10 Alat with sign
            &req.params[3],    // R
            &req.params[4],    // H1
            &req.params[5])    // H2
         != 6)
            return {false};

      auto& start = req.params[4];
      auto& end   = req.params[5];

      // time constrain
      if (start >= end)
            return {false};

      struct Ans
      {
            VM_Request      req;
            AVLTree<string> list;
            size_t          num_vehicle;

            Ans(VM_Request r) : req(r) {}
      };

      Ans ans(req);
      ans.num_vehicle = vehicles.getSize();

      try {
            list.traverse(
               [](VM_Record& r, void* v) {
                     auto ans  = (Ans*) v;
                     auto hour = gmtime(&r.timestamp)->tm_hour;

                     // variables for easy reading
                     auto& lon    = ans->req.params[1];
                     auto& lat    = ans->req.params[2];
                     auto& radius = ans->req.params[3];
                     auto& start  = ans->req.params[4];
                     auto& end    = ans->req.params[5];

                     if (start > hour || end < hour)
                           return;

                     auto distance = r.DistanceTo(lat, lon);

                     if (distance > radius)
                           return;

                     string id = r.id;
                     ans->list.insert(
                        id, [](string& s1, string& s2) { return s1 == s2; });

                     if (ans->num_vehicle == ans->list.getSize())
                           throw true;
               },
               &ans);
      } catch (bool& b) {
            // if all vehicles is in the ring between that time
            return {(int) vehicles.getSize()};
      }

      return {(int) ans.list.getSize()};
}
returnType request5(VM_Request& req, L1List<VM_Record>& list) {
      if (list.isEmpty())
            return {(int) 0};

      struct Ans
      {
            char   id[ID_MAX_LENGTH];
            double lon;
            double lat;
            double radius;
            int    occurence;
      };

      Ans ans;
      int req_id;

      if (
         sscanf(
            req.code,
            "%1d_%16[A-Za-z0-9]_%10lf_%10lf_%10lf",
            &req_id,
            ans.id,
            &ans.lon,
            &ans.lat,
            &ans.radius) != 5)
            return {false};

      list.traverse(
         [](VM_Record& r, void* v) {
               auto ans = (Ans*) v;
               if (strcmp(ans->id, r.id) != 0)
                     return;

               if (r.DistanceTo(ans->lat, ans->lon) <= ans->radius)
                     ans->occurence++;
         },
         &ans);

      return {ans.occurence};
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
