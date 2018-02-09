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

      auto  data        = (void**) pGData;
      auto  vehicles    = (AVLTree<string>*) data[0];
      auto  records     = (AVLTree<VM_Record>*) data[1];
      auto& restriction = (AVLTree<string>*&) data[2];

      size_t vehicles_size = vehicles->getSize();

      ReturnType* r = nullptr;

      switch (request.code[0] - '0') {
            case 1:
                  r = request1(request, *records, *restriction);
                  break;
            case 2:
                  r = request2(request, *records, vehicles_size, *restriction);
                  break;
            case 3:
                  r = request3(request, *records, vehicles_size, *restriction);
                  break;
            case 4:
                  r = request4(request, *records, vehicles_size, *restriction);
                  break;
            case 5:
                  r = request5(request, *records, *vehicles, *restriction);
                  break;
            case 6:
                  r = request6(request, *records, *restriction);
                  break;
            case 7:
                  r = request7(request, *records, *restriction);
                  break;
            case 8:
                  r = request8(request, *records, *restriction);
                  break;
            case 9:
                  r = request9(request, *records, restriction);
                  break;
            default:
                  break;
      }

      return print(r, request, *restriction);
}


ReturnType* request1(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    restriction) {

      if (records.isEmpty())
            return new ReturnType((int) -1);

      auto sample   = *(records.begin());
      auto thisTime = gmtime(&sample.timestamp);

      VM_Record car_1, car_2;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(
                s,
                "_%16[a-zA-Z0-9]_%16[a-zA-Z0-9]_%2d%2d%2d",
                car_1.id,              // id of vehicle number 1
                car_2.id,              // id of vehicle number 2
                &thisTime->tm_hour,    // hour
                &thisTime->tm_min,     // minute
                &thisTime->tm_sec)     // second
          != 5)
            return new ReturnType(false);

      car_1.timestamp = timegm(thisTime);
      car_2.timestamp = timegm(thisTime);

      // check in restriction
      string* found = nullptr;
      string  id_1(car_1.id);
      string  id_2(car_2.id);
      if (restriction.find(id_1, found) || restriction.find(id_2, found))
            return new ReturnType((int) -1);

      // check in records
      VM_Record* ret_1 = nullptr;
      VM_Record* ret_2 = nullptr;
      if (!records.find(car_1, ret_1) || !records.find(car_2, ret_2))
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
      const size_t&       vehicles_size,
      AVLTree<string>&    restriction) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

      char   direction;
      double lon;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(s, "_%lf_%1[EW]", &lon, &direction) != 2)
            return new ReturnType(false);

      AVLTree<string> result;
      for (auto& r : records) {
            string relative_lon = r.RelativeLongitudeTo(lon);
            if (relative_lon[0] != direction && relative_lon[0] != '?') {
                  string  id(r.id);
                  string* found = nullptr;

                  // check in restriction
                  if (restriction.find(id, found)) {
                        continue;
                  }

                  result.insert(id, [](string& Old, string& New) {
                        return Old == New;
                  });

                  if (result.getSize() == vehicles_size - restriction.getSize())
                        break;
            }
      }

      return new ReturnType(
            (int) (vehicles_size - result.getSize() - restriction.getSize()));
}

ReturnType* request3(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size,
      AVLTree<string>&    restriction) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

      char   direction;
      double lat;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(s, "_%lf_%1[NS]", &lat, &direction) != 2)
            return new ReturnType(false);

      AVLTree<string> result;
      for (auto& r : records) {
            string relative_lat = r.RelativeLatitudeTo(lat);
            if (relative_lat[0] != direction && relative_lat[0] != '?') {
                  string  id(r.id);
                  string* found = nullptr;

                  // check in restriction
                  if (restriction.find(id, found)) {
                        continue;
                  }

                  result.insert(id, [](string& Old, string& New) {
                        return Old == New;
                  });

                  if (result.getSize() == vehicles_size - restriction.getSize())
                        break;
            }
      }

      return new ReturnType(
            (int) (vehicles_size - result.getSize() - restriction.getSize()));
}

ReturnType* request4(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      const size_t&       vehicles_size,
      AVLTree<string>&    restriction) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

      double lon;
      double lat;
      double radius;
      int    start;
      int    end;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(s, "_%lf_%lf_%lf_%d_%d", &lon, &lat, &radius, &start, &end) !=
          5)
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
                  string  id(r.id);
                  string* found = nullptr;

                  // check in restriction
                  if (restriction.find(id, found)) {
                        continue;
                  }

                  result.insert(id, [](string& Old, string& New) {
                        return Old == New;
                  });

                  if (result.getSize() == vehicles_size - restriction.getSize())
                        break;
            }
      }
      return new ReturnType((int) result.getSize());
}

ReturnType* request5(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    vehicles,
      AVLTree<string>&    restriction) {

      if (records.isEmpty())
            return new ReturnType((int) 0);

      char   char_id[ID_MAX_LENGTH];
      double lat;
      double lon;
      double radius;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(
                s,
                "_%16[A-Za-z0-9]_%lf_%lf_%lf",
                char_id,
                &lon,
                &lat,
                &radius) != 4)
            return new ReturnType(false);

      string  id(char_id);
      string* ret = nullptr;
      if (!vehicles.find(id, ret) || restriction.find(id, ret)) {
            ret = nullptr;
            return new ReturnType(int(-1));
      }
      ret = nullptr;

      int  occurence = 0;
      bool isIn      = false;
      for (auto& r : records) {
            if (strcmp(r.id, char_id) != 0)
                  continue;

            double distance = r.DistanceTo(lat, lon);

            if (isIn && distance > radius) {
                  isIn = false;
            }
            else if (!isIn && distance <= radius) {
                  isIn = true;
                  occurence++;
            }
            else {
                  continue;
            }
      }

      return new ReturnType(occurence);
}
ReturnType* request6(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    restriction) {
      if (records.isEmpty()) {
            string ret = "-1 - -1";
            return new ReturnType(ret);
      }

      double lon;
      double lat;
      int    vehicles_inside;
      int    hour;
      int    minute;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(
                s,
                "_%lf_%lf_%d_%2d%2d",
                &lon,
                &lat,
                &vehicles_inside,
                &hour,
                &minute) != 5)
            return new ReturnType(false);

      auto under_2km  = new AVLTree<string>();
      auto under_300m = new AVLTree<string>();
      auto over_500m  = new AVLTree<string>();
      auto under_500m = new AVLTree<string>();

      for (auto& r : records) {
            auto open_time_tm     = gmtime(&r.timestamp);
            open_time_tm->tm_hour = hour;
            open_time_tm->tm_min  = minute;
            open_time_tm->tm_sec  = 0;

            auto open_time = timegm(open_time_tm);

            if (r.timestamp > open_time)
                  break;

            if (open_time - r.timestamp > 60 * 15)
                  continue;

            auto distance = r.DistanceTo(lat, lon);

            string  id(r.id);
            string* found = nullptr;

            if (restriction.find(id, found))
                  continue;

            auto cmp = [](string& Old, string& New) { return Old == New; };

            if (distance <= 0.3) {
                  under_300m->insert(id, cmp);
                  under_500m->insert(id, cmp);
                  under_2km->insert(id, cmp);
            }

            else if (distance <= 0.5) {
                  under_500m->insert(id, cmp);
                  under_2km->insert(id, cmp);
            }

            else if (distance <= 2) {
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

      if ((int) under_2km->getSize() < vehicles_inside) {
            // all in
            in  = under_2km;
            out = nullptr;

            delete under_300m;
            delete under_500m;
            delete over_500m;

            under_300m = nullptr;
            under_500m = nullptr;
            over_500m  = nullptr;
      }
      else if ((int) under_300m->getSize() > 0.75 * vehicles_inside) {
            // all out
            out = under_2km;
            in  = nullptr;

            delete under_300m;
            delete under_500m;
            delete over_500m;

            under_300m = nullptr;
            under_500m = nullptr;
            over_500m  = nullptr;
      }
      else {
            // if any vehicle in both 500 above and below, choose 500 in
            // that gives us a full in with 500 below
            in = under_500m;
            // besides, delete is odd, so we create new tree to make it easier
            // if it's slower, than we will delete
            out           = new AVLTree<string>();
            string* found = nullptr;
            for (auto& v : *over_500m) {
                  if (in->find(v, found))
                        continue;
                  out->insert(v);
            }
            found = nullptr;

            delete under_300m;
            delete under_2km;
            delete over_500m;

            under_300m = nullptr;
            under_2km  = nullptr;
            over_500m  = nullptr;
      }

      auto rt_out = new ReturnType(out);
      auto rt_in  = new ReturnType(in);
      list->insertHead(rt_out);
      list->insertHead(rt_in);

      // don't forget to delete unused data
      return new ReturnType(list);
}
ReturnType* request7(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    restriction) {
      if (records.isEmpty()) {
            string ret = "-1 - -1";
            return new ReturnType(ret);
      }

      double lon;
      double lat;
      int    vehicles_inside;
      double radius;
      int    hour;
      int    minute;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(
                s,
                "_%lf_%lf_%d_%lf_%2d%2d",
                &lon,
                &lat,
                &vehicles_inside,
                &radius,
                &hour,
                &minute) != 6)
            return new ReturnType(false);

      struct id_distance
      {
            string id;
            double distance;

            bool operator<(id_distance idis) {
                  return id < idis.id;
            }

            bool operator==(id_distance idis) {
                  return id == idis.id;
            }
      };

      AVLTree<id_distance> min_distance;
      AVLTree<id_distance> max_distance;
      for (auto& r : records) {
            auto open_time_tm     = gmtime(&r.timestamp);
            open_time_tm->tm_hour = hour;
            open_time_tm->tm_min  = minute;
            open_time_tm->tm_sec  = 0;

            auto open_time = timegm(open_time_tm);

            if (r.timestamp > open_time + 60 * 30)
                  break;

            if (r.timestamp < open_time)
                  continue;

            id_distance idis;
            idis.distance = r.DistanceTo(lat, lon);
            idis.id       = r.id;
            string* found = nullptr;

            if (restriction.find(idis.id, found))
                  continue;

            id_distance* found_re = nullptr;
            if (min_distance.find(idis, found_re)) {
                  if (idis.distance < found_re->distance)
                        found_re->distance = idis.distance;
            }
            else {
                  min_distance.insert(
                        idis, [](id_distance& Old, id_distance& New) {
                              return Old == New;
                        });
            }

            if (max_distance.find(idis, found_re)) {
                  if (idis.distance > found_re->distance)
                        found_re->distance = idis.distance;
            }
            else {
                  max_distance.insert(
                        idis, [](id_distance& Old, id_distance& New) {
                              return Old == New;
                        });
            }
      }

      auto under_500m = new AVLTree<string>();
      auto under_1km  = new AVLTree<string>();
      auto under_2km  = new AVLTree<id_distance>();

      for (auto& idis : min_distance) {
            if (idis.distance <= 0.5)
                  under_500m->insert(idis.id);
            if (idis.distance <= 1)
                  under_1km->insert(idis.id);
            if (idis.distance <= 2)
                  under_2km->insert(idis);
      }

      AVLTree<string>* in  = nullptr;
      AVLTree<string>* out = nullptr;

      auto make_string_tree =
            [](AVLTree<id_distance>* tree) -> AVLTree<string>* {
            if (tree == nullptr)
                  return nullptr;
            auto ret = new AVLTree<string>();
            for (auto& t : *tree)
                  ret->insert(t.id);
            return ret;
      };

      if (under_500m->getSize() < 0.7 * vehicles_inside) {
            out = make_string_tree(under_2km);
            in  = nullptr;

            delete under_500m;
            delete under_1km;
            delete under_2km;

            under_500m = nullptr;
            under_1km  = nullptr;
            under_2km  = nullptr;
      }
      else {
            // in  = under_1km;    // and 25% of 1-2km
            in  = under_1km;
            out = new AVLTree<string>();

            struct distance_id
            {
                  string id;
                  double distance;

                  bool operator<(distance_id disid) {
                        // because we want max -> min
                        // in LNR order
                        return distance > disid.distance;
                  }

                  bool operator==(distance_id disid) {
                        return distance == disid.distance;
                  }
            };

            // get list id by distance order LNR max->min
            // only id with distance > 1 and <= 2
            // because we add all <= 1 in IN list already
            AVLTree<distance_id> distance_order;
            for (auto& idis : *under_2km) {
                  if (idis.distance <= 1)
                        continue;
                  if (idis.distance > 2)
                        continue;
                  string* found = nullptr;
                  if (in->find(idis.id, found))
                        continue;

                  distance_id disid;
                  disid.id       = idis.id;
                  disid.distance = idis.distance;
                  distance_order.insert(disid);
            }

            int size  = distance_order.getSize();
            int count = 0;
            for (auto& disid : distance_order) {
                  if (count++ <= (double) (0.75 * size)) {
                        // out->insert(disid.id);
                  }
                  else {
                        // in->insert(disid.id);
                  }
            }

            delete under_500m;
            delete under_2km;

            under_500m = nullptr;
            under_1km  = nullptr;
            under_2km  = nullptr;
      }

      auto rt_out = new ReturnType(out);
      auto rt_in  = new ReturnType(in);
      auto list   = new L1List<ReturnType*>();
      list->insertHead(rt_out);
      list->insertHead(rt_in);

      // don't forget to delete unused data
      return new ReturnType(list);
}
ReturnType* request8(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>&    restriction) {

      if (records.isEmpty()) {
            string ret = "-1";
            return new ReturnType(ret);
      }

      double lon;
      double lat;
      double radius;
      int    hour;
      int    minute;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(
                s,
                "_%lf_%lf_%lf_%2d%2d",
                &lon,
                &lat,
                &radius,
                &hour,
                &minute) != 5)
            return new ReturnType(false);

      auto    this_restriction = new AVLTree<string>();
      string* found            = nullptr;
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

            if (restriction.find(id, found))
                  continue;

            this_restriction->insert(
                  id, [](string& Old, string& New) { return Old == New; });
            restriction.insert(
                  id, [](string& Old, string& New) { return Old == New; });
      }
      found = nullptr;

      // this_restriction is for print out and stuff
      // restriction is for checking with other request
      // thus we need restriction to be avaiable all the time
      return new ReturnType(this_restriction);
}
ReturnType* request9(
      VM_Request&         req,
      AVLTree<VM_Record>& records,
      AVLTree<string>*&   restriction) {
      if (records.isEmpty()) {
            string ret = "-1";
            return new ReturnType(ret);
      }

      double lon;
      double lat;
      double radius;
      int    hour;
      int    minute;

      auto s = strstr(req.code, "_");
      if (s == nullptr)
            return new ReturnType(false);
      if (sscanf(
                s,
                "_%lf_%lf_%lf_%2d%2d",
                &lon,
                &lat,
                &radius,
                &hour,
                &minute) != 5)
            return new ReturnType(false);

      auto resurrect = new AVLTree<string>();
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
            resurrect->insert(
                  id, [](string& Old, string& New) { return Old == New; });
      }

      auto    new_restriction = new AVLTree<string>();
      auto    PrintList       = new AVLTree<string>();
      string* found           = nullptr;
      for (auto& r : *restriction) {
            if (resurrect->find(r, found)) {
                  PrintList->insert(
                        r, [](string& Old, string& New) { return Old == New; });
            }
            else {
                  new_restriction->insert(
                        r, [](string& Old, string& New) { return Old == New; });
            }
      }
      delete restriction;
      restriction = new_restriction;

      return new ReturnType(PrintList);
}


bool print(ReturnType* r, VM_Request& req, AVLTree<string>& restriction) {
      if (r == nullptr)
            return false;

      auto PrintRequestCode = [](VM_Request& req) {
            for (size_t i = 0; req.code[i] != '_'; i++)
                  cout << req.code[i];
            cout << ":";
      };

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
                  PrintRequestCode(req);

                  if (r->l->isEmpty())
                        cout << " -1";

                  else if (req.code[0] == '1')
                        // list of request 1
                        for (auto& x : *(r->l))
                              cout << *x;

                  else {
                        // list of tree ids
                        // <tree 1> - <tree 2>
                        // check in restriction
                        auto it = r->l->begin();
                        //    *it ---- ReturnType*
                        // *(*it) ---- ReturnType
                        cout << *(*(it++));
                        cout << " -";
                        cout << *(*it);
                  }

                  cout << "\n";
                  delete r;
                  return true;

            default:
                  // int
                  // double
                  // tree as list
                  if ((req.code[0] == '8' || req.code[0] == '9') &&
                      r->tr->isEmpty()) {
                        PrintRequestCode(req);
                        cout << " 0\n";
                  }
                  else {
                        PrintRequestCode(req);
                        cout << *r << "\n";
                  }
                  delete r;
                  return true;
      }

      return false;
}
