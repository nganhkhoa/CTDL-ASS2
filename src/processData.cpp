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
      auto vehicles    = new AVLTree<string>();
      auto records     = new AVLTree<VM_Record>();
      auto restriction = new AVLTree<VM_Record>();
      auto rList       = (L1List<VM_Record>*) *pGData;

      for (auto& vmr : *rList) {
            string id(vmr.id);
            vehicles->insert(
                  id, [](string& Old, string& New) { return Old == New; });

            VM_Record r = vmr;
            records->insert(r);
      }

      auto data = new void*[3];
      data[0]   = vehicles;
      data[1]   = records;
      data[2]   = restriction;

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
      auto data        = (void**) pGData;
      auto vehicles    = (AVLTree<string>*) data[0];
      auto records     = (AVLTree<VM_Record>*) data[1];
      auto restriction = (AVLTree<string>*) data[2];

      delete restriction;
      delete vehicles;
      delete records;
      delete data;

      restriction = nullptr;
      vehicles    = nullptr;
      records     = nullptr;
      data        = nullptr;
      pGData      = nullptr;
}


bool print(ReturnType*, VM_Request&, AVLTree<string>&);

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

      auto data        = (void**) pGData;
      auto vehicles    = (AVLTree<string>*) data[0];
      auto records     = (AVLTree<VM_Record>*) data[1];
      auto restriction = (AVLTree<string>*) data[2];

      size_t vehicles_size = vehicles->getSize();

      ReturnType* r = nullptr;

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
                  r = request7(request, *records);
                  break;
            case 8:
                  r = request8(request, *records, *restriction);
                  break;
            case 9:
                  r = request9(request, *records, *restriction);
                  break;
            default:
                  break;
      }

      return print(r, request, *restriction);
}


ReturnType* request1(VM_Request& request, AVLTree<VM_Record>& records) {

      if (records.isEmpty())
            return new ReturnType((int) -1);

      auto sample   = *(records.begin());
      auto thisTime = gmtime(&sample.timestamp);

      if (thisTime == nullptr)
            return new ReturnType("cannot get time from data");

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
            return new ReturnType(false);

      car_1.timestamp = timegm(thisTime);
      car_2.timestamp = timegm(thisTime);

      VM_Record* ret_1 = nullptr;
      VM_Record* ret_2 = nullptr;

      records.find(car_1, ret_1);
      records.find(car_2, ret_2);

      if (ret_1 == nullptr || ret_2 == nullptr)
            return new ReturnType((int) -1);

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

      return new ReturnType(ret);
}

ReturnType* request2(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

      char   direction;
      int    req_id;
      double lon;
      if (sscanf(req.code, "%1d_%lf_%1[EW]", &req_id, &lon, &direction) != 3)
            return new ReturnType(false);

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

      return new ReturnType((int) (vehicles_size - result.getSize()));
}

ReturnType* request3(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

      char   direction;
      int    req_id;
      double lat;
      if (sscanf(req.code, "%1d_%lf_%1[NS]", &req_id, &lat, &direction) != 3)
            return new ReturnType(false);

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

      return new ReturnType((int) (vehicles_size - result.getSize()));
}

ReturnType* request4(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

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
            return new ReturnType(false);

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
      return new ReturnType((int) result.getSize());
}

ReturnType* request5(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    vehicles) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

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
            return new ReturnType(false);

      string  id(char_id);
      string* ret;
      if (!vehicles.find(id, ret)) {
            ret = nullptr;
            return new ReturnType(int(0));
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

      return new ReturnType(occurence);
}
ReturnType* request6(VM_Request& req, AVLTree<VM_Record>& records) {
      if (records.isEmpty()) {
            string ret = "-1 - -1";
            return new ReturnType(ret);
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
            return new ReturnType(false);

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

            if (distance < 0.3) {
                  under_300m->insert(id, cmp);
                  under_500m->insert(id, cmp);
                  under_2km->insert(id, cmp);
            }

            else if (distance < 0.5) {
                  under_500m->insert(id, cmp);
                  under_2km->insert(id, cmp);
            }

            else if (distance < 2) {
                  over_500m->insert(id, cmp);
                  under_2km->insert(id, cmp);
            }

            else {
                  continue;
            }
      }

      AVLTree<string>* out  = nullptr;
      AVLTree<string>* in   = nullptr;
      auto             list = new L1List<ReturnType*>();

      // again, what is "in" and "out"
      // is that only the vehicles in 15' prior to the time
      // or all vehicles
      // i have a confirm on that it is only 2km below
      if ((int) under_2km->getSize() < vehicles_inside) {
            // all in
            in  = under_2km;
            out = nullptr;

            delete under_300m;
            delete under_500m;
            delete over_500m;
      }
      else if ((int) under_300m->getSize() >= 0.75 * vehicles_inside) {
            // all out
            out = under_2km;
            in  = nullptr;

            delete under_300m;
            delete under_500m;
            delete over_500m;
      }
      else {
            // under 500 is in
            // over 500 is out
            // what about both in and out?
            // we only need to have one coordinate there
            // what stupid
      }

      auto rt_out = new ReturnType(out);
      auto rt_in  = new ReturnType(in);
      list->insertHead(rt_out);
      list->insertHead(rt_in);

      // don't forget to delete data unused
      return new ReturnType(list);
}
ReturnType* request7(VM_Request& req, AVLTree<VM_Record>& records) {
      return new ReturnType(false);
}
ReturnType* request8(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    restriction) {

      if (records.isEmpty()) {
            string ret = "-1";
            return new ReturnType(ret);
      }

      int    req_id;
      double lon;
      double lat;
      double radius;
      int    hour;
      int    minute;

      if (sscanf(
                req.code,
                "%1d_%lf_%lf_%lf_%2d%2d",
                &req_id,
                &lon,
                &lat,
                &radius,
                &hour,
                &minute) != 6)
            return new ReturnType(false);

      auto this_restriction = new AVLTree<string>();
      for (auto& r : records) {
            struct tm* tm = gmtime(&r.timestamp);
            if (tm->tm_hour > hour)
                  break;

            if (tm->tm_hour < hour)
                  continue;
            if (tm->tm_min != minute)
                  continue;

            if (r.DistanceTo(lat, lon) > radius)
                  continue;

            string id(r.id);
            this_restriction->insert(
                  id, [](string& Old, string& New) { return Old == New; });
            restriction.insert(
                  id, [](string& Old, string& New) { return Old == New; });
      }

      // this_restriction is for print out and stuff
      // restriction is for checking with other request
      // thus we need restriction to be avaiable all the time
      return new ReturnType(this_restriction);
}
ReturnType* request9(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    restriction) {
      return new ReturnType(false);
}


bool print(ReturnType* r, VM_Request& req, AVLTree<string>& restriction) {
      if (r == nullptr)
            return false;

      switch (r->t) {
            case ReturnType::type::empty:
                  delete r;
                  return false;

            case ReturnType::type::boolean:
                  delete r;
                  return false;

            case ReturnType::type::error:
                  cout << *r;
                  delete r;
                  return false;

            case ReturnType::type::list:
                  cout << req.code[0] << ":";

                  if (r->l->isEmpty())
                        cout << " -1";

                  else if (req.code[0] == 1)
                        // list of request 1
                        for (auto& x : *(r->l))
                              cout << " " << x;

                  else {
                        // list of tree ids
                        // <tree 1> - <tree 2>
                        // check in restriction
                        auto it = r->l->begin();
                        (*(it++))->printTreeWithRestriction(restriction);
                        cout << " -";
                        (*it)->printTreeWithRestriction(restriction);
                  }

                  cout << "\n";
                  delete r;
                  return true;

            default:
                  // int
                  // double
                  // tree as list
                  cout << req.code[0] << ":" << *r << "\n";
                  delete r;
                  return true;
      }

      return false;
}
