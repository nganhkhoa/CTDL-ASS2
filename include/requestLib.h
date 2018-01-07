/*
 * =========================================================================================
 * Name        : requestLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa
 * University Description : library for Assignment 2 - Data structures and
 * Algorithms - Fall 2017 This library contains functions used for event
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
 * Library use with define so no affect on building online
 *
 * Libraries used:
 *    Spdlog: https://github.com/gabime/spdlog
 *    Googletest: https://github.com/google/googletest
 *
 * This is only in case teacher want to interview me
 */

#ifndef DSA171A2_REQUESTLIB_H
#define DSA171A2_REQUESTLIB_H

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>

#include <dsaLib.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define REQUEST_CODE_SIZE 100
#define MAX_PARAM_SIZE 6
using namespace std;

typedef struct VM_Request
{
      char   code[REQUEST_CODE_SIZE];
      double params[MAX_PARAM_SIZE];

      VM_Request() {
            *code   = '0';    // default event is "0"
            code[1] = 0;
      }
      VM_Request(const char* str) {
            strncpy(code, str, REQUEST_CODE_SIZE - 1);
      }
      VM_Request(string& str) {
            strncpy(code, str.data(), REQUEST_CODE_SIZE - 1);
      }
      VM_Request(VM_Request& a) {    // copy constructor
            memcpy(code, a.code, REQUEST_CODE_SIZE);
            memcpy(params, a.params, MAX_PARAM_SIZE * sizeof(double));
      }

      VM_Request(VM_Request&& a) {    // move constructor
            int i = 0;
            while (a.code[i]) {
                  code[i] = a.code[i];
                  i++;
            }
            code[i] = 0;
      }

      bool operator==(VM_Request& b) {
            return strcmp(code, b.code) == 0;
      }
} VM_Request;


struct returnType
{
      /**
       * Return with type empty or boolean
       *    => failed or request wrong
       *    => which all comes to wrong request being read
       *    => print -1
       *
       * Return with type double
       *    => print with setprecision(12)
       *
       * Return with type number
       *    => print number
       *
       * Return with type list
       *    => print the items
       *
       * Return with type tree
       *    => print by traversing LNR
       *    => sorted data printing
       */
      enum class type : int
      {
            empty,
            error,
            list,
            tree,
            boolean,
            floatingpoint,
            number
      };
      union {                        // 4 bytes
            L1List<string>*  l;      // 4 bytes
            AVLTree<string>* tr;     // 4 bytes
            bool             b;      // 2 bytes
            double           d;      // 4 bytes
            int              i;      // 4 bytes
            char*            err;    // 4 bytes
      };
      type t;

      // ok, this struct has 2 members
      // type t, and an unnamed union
      // shallow copy is find though
      // no need to define copy cons

      returnType() {
            t = type::empty;
      }

      returnType(L1List<string>* l) {
            t       = type::list;
            this->l = l;
      }

      returnType(AVLTree<string>* tr) {
            t        = type::tree;
            this->tr = tr;
      }

      returnType(bool b) {
            t       = type::boolean;
            this->b = b;
      }

      returnType(double d) {
            t       = type::floatingpoint;
            this->d = d;
      }

      returnType(int i) {
            t       = type::number;
            this->i = i;
      }

      returnType(const char* err) {
            t = type::error;
            strcpy(this->err, err);
      }
};

void loadRequests(char* fName, L1List<VM_Request>&);

#endif    // DSA171A2_REQUESTLIB_H
