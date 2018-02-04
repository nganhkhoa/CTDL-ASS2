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
      auto vehicles = new AVLTree<string>();
      auto records  = new AVLTree<VM_Record>();
      auto rList    = (L1List<VM_Record>*) *pGData;

      for (auto& vmr : *rList) {
            string id(vmr.id);
            vehicles->insert(
                  id, [](string& Old, string& New) { return Old == New; });

            VM_Record r = vmr;
            records->insert(r);
      }

      auto data = new void*[2];
      data[0]   = vehicles;
      data[1]   = records;

      *pGData = data;

#ifdef DEBUGGING
      auto console = spdlog::get("console.log");
      auto file    = spdlog::get("file.log");
      console->info("{} vehicles", vehicles->getSize());
      file->info("{} vehicles", vehicles->getSize());

      string str;
      for (auto& s : *vehicles) {
            str += " " + s;
      }

      file->info("vehicles:\n{}", str);
#endif

      return true;
}

void releaseVMGlobalData(void* pGData) {
      auto data     = (void**) pGData;
      auto vehicles = (AVLTree<string>*) data[0];
      auto records  = (AVLTree<VM_Record>*) data[1];

      delete vehicles;
      delete records;
      delete data;

      vehicles = nullptr;
      records  = nullptr;
      data     = nullptr;
      pGData   = nullptr;
}


bool print(ReturnType&, VM_Request&);

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

      auto data     = (void**) pGData;
      auto vehicles = (AVLTree<string>*) data[0];
      auto records  = (AVLTree<VM_Record>*) data[1];

      size_t vehicles_size = vehicles->getSize();

      ReturnType r;

      switch (request.code[0] - '0') {
            case 1:
                  r = request1(request, *records);
                  break;
            case 2:
                  r = request2(request, *records, vehicles_size);
                  break;
            case 3:
                  r = request3(request, *records, vehicles_size);
                  break;
            case 4:
                  r = request4(request, *records, vehicles_size);
                  break;
            case 5:
                  r = request5(request, *records, *vehicles);
                  break;
            case 6:
                  r = request6(request, *records);
                  break;
            case 7:
                  r = request7(request);
                  break;
            case 8:
                  r = request8(request);
                  break;
            case 9:
                  r = request9(request);
                  break;
            default:
                  break;
      }

      return print(r, request);
}


ReturnType request1(VM_Request& request, AVLTree<VM_Record>& records) {

      if (records.isEmpty())
            return {(int) -1};

      auto sample   = *(records.begin());
      auto thisTime = gmtime(&sample.timestamp);

      if (thisTime == nullptr)
            return {"cannot get time from data"};

      VM_Record car_1, car_2;

      int req_id;
      if (sscanf(    // 1_X_Y_hhmmss
                request.code,
                "%1d_%16[a-zA-Z0-9]_%16[a-zA-Z0-9]_%2d%2d%2d",
                &req_id,               // request id
                car_1.id,              // id of vehicle number 1
                car_2.id,              // id of vehicle number 2
                &thisTime->tm_hour,    // hour
                &thisTime->tm_min,     // minute
                &thisTime->tm_sec)     // second
          != 6)
            return {false};

      car_1.timestamp = timegm(thisTime);
      car_2.timestamp = timegm(thisTime);

      VM_Record* ret_1 = nullptr;
      VM_Record* ret_2 = nullptr;

      records.find(car_1, ret_1);
      records.find(car_2, ret_2);

      if (ret_1 == nullptr || ret_2 == nullptr)
            return {(int) -1};

      string relative_lat = ret_1->RelativeLatitudeTo(*ret_2);
      string relative_lon = ret_1->RelativeLongitudeTo(*ret_2);
      double relative_dis = ret_1->DistanceTo(*ret_2);

      ReturnType* lat      = new ReturnType(relative_lat);
      ReturnType* lon      = new ReturnType(relative_lon);
      ReturnType* distance = new ReturnType(relative_dis);

      auto ret = new L1List<ReturnType*>();
      ret->insertHead(distance);
      ret->insertHead(lat);
      ret->insertHead(lon);

      return {ret};
}

ReturnType request2(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size) {

      if (records.isEmpty())
            return {(int) 0};

      char   direction;
      int    req_id;
      double lon;
      if (sscanf(req.code, "%1d_%lf_%1[EW]", &req_id, &lon, &direction) != 3)
            return {false};

      AVLTree<string> result;
      for (auto& r : records) {
            string relative_lon = r.RelativeLongitudeTo(lon);
            if (relative_lon[0] != direction) {
                  string id(r.id);
                  result.insert(id, [](string& Old, string& New) {
                        return Old == New;
                  });

                  if (result.getSize() == vehicles_size)
                        break;
            }
      }

      return {(int) (vehicles_size - result.getSize())};
}

ReturnType request3(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size) {

      if (records.isEmpty())
            return {(int) 0};

      char   direction;
      int    req_id;
      double lat;
      if (sscanf(req.code, "%1d_%lf_%1[NS]", &req_id, &lat, &direction) != 3)
            return {false};

      AVLTree<string> result;
      for (auto& r : records) {
            string relative_lat = r.RelativeLatitudeTo(lat);
            if (relative_lat[0] != direction) {
                  string id(r.id);
                  result.insert(id, [](string& Old, string& New) {
                        return Old == New;
                  });

                  if (result.getSize() == vehicles_size)
                        break;
            }
      }

      return {(int) (vehicles_size - result.getSize())};
}

ReturnType request4(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size) {

      if (records.isEmpty())
            return {(int) 0};

      int    req_id;
      double lon;
      double lat;
      double radius;
      int    start;
      int    end;

      if (sscanf(
                req.code,
                "%1d_%lf_%lf_%lf_%d_%d",
                &req_id,
                &lon,
                &lat,
                &radius,
                &start,
                &end) != 6)
            return {false};

      AVLTree<string> result;
      for (auto& r : records) {
            int hour = gmtime(&r.timestamp)->tm_hour;
            if (hour < start)
                  continue;

            if (hour >= end)
                  // our tree is build with time order
                  // if time is bigger then we need a
                  break;

            double distance = r.DistanceTo(lat, lon);

            if (distance <= radius) {
                  string id(r.id);
                  result.insert(id, [](string& Old, string& New) {
                        return Old == New;
                  });

                  if (result.getSize() == vehicles_size)
                        break;
            }
      }
#ifdef DEBUGGING
      auto file = spdlog::get("file.log");
      file->info("Request 4: {} vehicles", result.getSize());

      string str;
      for (auto& s : result) {
            str += " " + s;
      }

      file->info("vehicles:\n{}", str);
#endif
      return {(int) result.getSize()};
}

ReturnType request5(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    vehicles) {

      if (records.isEmpty())
            return {(int) 0};

      int    req_id;
      char   char_id[ID_MAX_LENGTH];
      double lat;
      double lon;
      double radius;

      if (sscanf(
                req.code,
                "%1d_%16[A-Za-z0-9]_%lf_%lf_%lf",
                &req_id,
                char_id,
                &lon,
                &lat,
                &radius) != 5)
            return {false};

      string  id(char_id);
      string* ret;
      if (!vehicles.find(id, ret)) {
            ret = nullptr;
            return {int(0)};
      }
      ret = nullptr;


      int occurence = 0;
      for (auto& r : records) {
            if (strcmp(r.id, char_id) != 0)
                  continue;

            double distance = r.DistanceTo(lat, lon);

            if (distance <= radius)
                  occurence++;
      }

      return {occurence};
}
ReturnType request6(VM_Request& req, AVLTree<VM_Record>& records) {
      if (records.isEmpty()) {
            string ret = "-1 - -1";
            return {ret};
      }

      int    req_id;
      double lon;
      double lat;
      int    vehicles_inside;
      int    hour;
      int    minute;

      if (sscanf(
                req.code,
                "%1d_%lf_%lf_%d_%2d%2d",
                &req_id,
                &lon,
                &lat,
                &vehicles_inside,
                &hour,
                &minute) != 6)
            return {false};

      auto under_2km  = new AVLTree<string>();
      auto under_300m = new AVLTree<string>();
      auto over_500m  = new AVLTree<string>();
      auto under_500m = new AVLTree<string>();

      for (auto& r : records) {
            auto open_time_tm     = gmtime(&r.timestamp);
            open_time_tm->tm_hour = hour;
            open_time_tm->tm_min  = minute;

            auto open_time = timegm(open_time_tm);

            if (r.timestamp < open_time)
                  break;

            if (r.timestamp - open_time > 60 * 15)
                  continue;

            auto distance = r.DistanceTo(lat, lon);

            string id(r.id);
            auto   cmp = [](string& Old, string& New) { return Old == New; };

            if (distance < 300) {
                  under_2km->insert(id, cmp);
                  under_300m->insert(id, cmp);
                  under_500m->insert(id, cmp);
            }

            else if (distance < 500) {
                  under_500m->insert(id, cmp);
                  under_2km->insert(id, cmp);
            }

            else if (distance < 2000) {
                  over_500m->insert(id, cmp);
                  under_2km->insert(id, cmp);
            }

            else {
                  continue;
            }
      }

      auto out  = new AVLTree<string>();
      auto in   = new AVLTree<string>();
      auto list = new L1List<ReturnType*>();

      // again, what is "in" and "out"
      // is that only the vehicles in 15' prior to the time
      // or all vehicles
      if ((int) under_2km->getSize() < vehicles_inside) {
            // all in
      }
      else if ((int) under_300m->getSize() >= 0.75 * vehicles_inside) {
            // all out
      }
      else {
            // under 500 is in
            // over 500 is out
      }

      // don't forget to delete data unused
      return {list};
}
ReturnType request7(VM_Request& req) {
      return {false};
}
ReturnType request8(VM_Request& req) {
      return {false};
}
ReturnType request9(VM_Request& req) {
      return {false};
}


bool print(ReturnType& r, VM_Request& req) {
      switch (r.t) {
            case ReturnType::type::empty:
                  return false;

            case ReturnType::type::boolean:
                  return false;

            case ReturnType::type::error:
                  cout << r;
                  return false;

            case ReturnType::type::list:
                  cout << req.code[0] << ":";

                  if (r.l->isEmpty())
                        cout << " -1";

                  else if (req.code[0] == 1)
                        // list of request 1
                        for (auto& x : *r.l)
                              cout << " " << x;

                  else
                        // list of tree ids
                        // <tree 1> - <tree 2>
                        for (auto& x : *r.l)
                              cout << x << " -";

                  cout << "\n";
                  return true;


            default:
                  cout << req.code[0] << ": " << r << "\n";
                  return true;
      }

      return false;
}
