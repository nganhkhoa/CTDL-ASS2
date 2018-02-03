#!/bin/bash

pwd='/home/luibo/Desktop/dsa171a2'

cowsay "Building...."
cd $pwd/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUNIT_TEST=OFF 1> /dev/null
make 1> /dev/null
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUNIT_TEST=ON 2> /dev/null 1> /dev/null
cd $pwd

cowsay "The request file data: $(cat $pwd/resource/request.txt)"

cowsay "Running my app...."
$pwd/bin/dsa171a2 $pwd/resource/request.txt $pwd/resource/data.csv > $pwd/testapp/my.log

cowsay "Running teacher's app...."
$pwd/testapp/dsa171a2 $pwd/resource/request.txt $pwd/resource/data.csv 2> /dev/null 1> $pwd/testapp/teacher.log

cowsay "The result is:"
diff $pwd/testapp/my.log $pwd/testapp/teacher.log

if [ $? -eq 0 ]; then
      echo "There's no difference, you are good to go"
      cowsay "Good job"
fi

