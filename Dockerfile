FROM rikorose/gcc-cmake

RUN   git clone https://github.com/nganhkhoa/CTDL-ASS2.git \
      && cd CTDL-ASS2 \
      && mkdir build && cd build \
      && cmake .. -DCMAKE_BUILD_TYPE=Release \
      && make \
      && cd ../bin \
      && mkdir logs \
      && ./dsa171a2 ../resource/request.txt ../resource/data.csv