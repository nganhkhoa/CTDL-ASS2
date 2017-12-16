FROM rikorose/gcc-cmake

RUN   ls \
      && mkdir build && cd build \
      && cmake .. -DCMAKE_BUILD_TYPE=Release \
      && make \
      && cd ../bin \
      && ./dsa171a2 ../resource/request.txt ../resource/data.csv