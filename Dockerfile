FROM rikorose/gcc-cmake

COPY . /dsa171a2/

RUN   ls /dsa171a2/ && cd /dsa171a2/ \
      && ./make-compress.sh false \
      && cd out \
      && g++ main.cpp -o main.o \
      && g++ dbLib.cpp -o dbLib.o \
      && g++ requestLib.cpp -o requestLib.o \
      && g++ processData.cpp -o processData.o \
      && g++ main.o dbLib.o requestLib.o processData.o -o /dsa171a2/bin/dsa171a2 \
      && rm -rf src/ include/ test/ out/

ENTRYPOINT ["/dsa171a2/bin/dsa171a2", "dsa171a2/resource/request.txt", "dsa171a2/resource/data.csv"]
