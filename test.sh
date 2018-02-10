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
my_start=$(date +%s)
$pwd/bin/dsa171a2 $pwd/resource/request.txt $pwd/resource/data.csv > $pwd/testapp/my.log
my_end=$(date +%s)

cowsay "Running teacher's app...."
teacher_start=$(date +%s)
$pwd/testapp/dsa171a2 $pwd/resource/request.txt $pwd/resource/data.csv 2> /dev/null 1> $pwd/testapp/teacher.log
teacher_end=$(date +%s)

cowsay "The result is:"
diff $pwd/testapp/my.log $pwd/testapp/teacher.log

if [ $? -eq 0 ]; then
      echo "There's no difference, you are good to go"
      cowsay "Good job"

      my_time=$(($my_end-$my_start))
      teacher_time=$(($teacher_end-$teacher_start))
      diff_time=$(($teacher_time - $my_time))

      echo "Your time:       $my_time"
      echo "Teacher time:    $teacher_time"
      echo "Time difference: $diff_time"
fi

