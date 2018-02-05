#!/bin/bash

pwd=$(pwd)  # get file path

cowsay "Building...."
cd $pwd/build
make clean
cmake .. -DCMAKE_BUILD_TYPE=Release -DUNIT_TEST=OFF 1> /dev/null
make 1> /dev/null
if [ $? -ne 0 ]; then
      failed=1
fi
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUNIT_TEST=ON 2> /dev/null 1> /dev/null
cd $pwd

if [ $failed -eq 1 ]; then
      cowsay -d "Sorry, build's Failed"
      exit 1
fi

if [ $# -eq 2 ]; then
      request=$1
      data=$2
elif [ $# -eq 1 ]; then
      request=$1
      data=$pwd'/resource/data.csv'
else
      request=$pwd'/resource/request.txt'
      data=$pwd'/resource/data.csv'
fi

cowsay "The request file data: $(cat $request)"

cowsay "Running my app...."
my_start=$(date +%s)
$pwd/bin/dsa171a2 $request $data > $pwd/testapp/my.log
my_end=$(date +%s)

cowsay "Running teacher's app...."
teacher_start=$(date +%s)
$pwd/testapp/dsa171a2 $request $data 2> /dev/null 1> $pwd/testapp/teacher.log
teacher_end=$(date +%s)

cowsay "The result is:"
diff $pwd/testapp/my.log $pwd/testapp/teacher.log

if [ $? -eq 0 ]; then
      echo "There's no difference, you are good to go"
      cowsay -e "^^" "Good job"

      my_time=$(($my_end-$my_start))
      teacher_time=$(($teacher_end-$teacher_start))
      diff_time=$(($teacher_time - $my_time))

      echo "Your time:       $my_time s"
      echo "Teacher time:    $teacher_time s"
      echo "Time difference: $diff_time s"
fi

