FROM rikorose/gcc-cmake

COPY src /dsa171a2/src
COPY include /dsa171a2/include
COPY resource /dsa171a2/resource
COPY CMakeLists.txt /dsa171a2/

RUN   ls /dsa171a2/ && cd /dsa171a2/ \
      && mkdir build && cd build \
      && cmake .. -DCMAKE_BUILD_TYPE=Release \
      && make

ENTRYPOINT ["/dsa171a2/bin/dsa171a2", "dsa171a2/resource/request.txt", "dsa171a2/resource/data.csv"]