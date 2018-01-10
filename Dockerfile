FROM rikorose/gcc-cmake

COPY . /dsa171a2/

RUN   apt-get -qq update \
      # get zip to make it work with script
      && apt-get -qq -y install zip \
      #
      # Start the job
      #
      && cd /dsa171a2/ \
      # make output dir first
      && mkdir -p bin/ && mkdir -p bin/logs/ \
      # run auto script
      && ./make-compress.sh false \
      && cd /dsa171a2/out/ \
      #
      # get all files needed to build
      # and build as using automatic online judge
      #
      && curl "https://raw.githubusercontent.com/saitamandd/DSA171-A02/master/main.cpp" > main.cpp \
      && g++ main.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ dbLib.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ requestLib.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ processData.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      #
      # link the built files
      #
      && g++ main.o dbLib.o requestLib.o processData.o -std=c++11 -DAVL_USE_HEIGHT -o /dsa171a2/bin/dsa171a2 \
      #
      # if build is ok then build and run tests
      #
      && mkdir -p /dsa171a2/build/ cd /dsa171a2/build/ \
      && cmake .. -DCMAKE_BUILD_TYPE=Release -DUNIT_TEST=ON > /dev/null \
      && make dsa171a2-test \
      && cd /dsa171a2/bin \
      # run tests
      && ./dsa171a2-test \
      #
      # If test is ok, then run the project
      # to see the output
      #
      && ./dsa171a2 /dsa171a2/resource/request.txt /dsa171a2/resource/data.csv \
      #
      # go back and delete source files
      #
      && cd /dsa171a2/ \
      # remove the folders so no one can see it
      && rm -rf src/ include/ test/ out/ build/

ENTRYPOINT ["/dsa171a2/bin/dsa171a2"]

CMD [ "/dsa171a2/resource/request.txt", "/dsa171a2/resource/data.csv" ]
