FROM rikorose/gcc-cmake

COPY . /dsa171a2/

# get zip to make it work with script
RUN   apt-get -qq update && apt-get -qq -y install zip \
      && cd /dsa171a2/ \
      # run auto script
      && ./make-compress.sh false \
      && cd out \
      # get all files needed to build
      && curl "https://raw.githubusercontent.com/saitamandd/DSA171-A02/master/main.cpp" > main.cpp \
      && g++ main.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ dbLib.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ requestLib.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ processData.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      # link the built files
      && g++ main.o dbLib.o requestLib.o processData.o -std=c++11 -DAVL_USE_HEIGHT -o /dsa171a2/bin/dsa171a2 \
      # remove the folders so no one can see it
      && rm -rf src/ include/ test/ out/

ENTRYPOINT ["/dsa171a2/bin/dsa171a2"]

CMD [ "dsa171a2/resource/request.txt", "dsa171a2/resource/data.csv" ]
