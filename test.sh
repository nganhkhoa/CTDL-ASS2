#!/bin/bash

pwd=$(pwd)  # get file path

cowsay "Building...."
cd $pwd/build
make clean
cmake .. -DCMAKE_BUILD_TYPE=Release -DUNIT_TEST=OFF 1> /dev/null
make 1> /dev/null
failed=0
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

mylog=$pwd'/testapp/my.log'
teacherlog=$pwd'/testapp/teacher.log'
difflog=$pwd'/testapp/diff.log'

cowsay "Running my app...."
my_start=$(date +%s)
$pwd/bin/dsa171a2 $request $data > $mylog
if [ $? -ne 0 ]; then
      cowsay -d "My app run failed, I should have check it"
      exit 1
fi
my_end=$(date +%s)

cowsay "Running teacher's app...."
teacher_start=$(date +%s)
$pwd/testapp/dsa171a2 $request $data 2> /dev/null 1> $teacherlog
if [ $? -ne 0 ]; then
      cowsay -d "Teacher's app run failed. Aha, teacher has a bug"
      exit 1
fi
teacher_end=$(date +%s)

echo "----------Request file----------" > $difflog
echo "" >> $difflog
cat $request >> $difflog
echo "" >> $difflog
echo "----------Your result-----------" >> $difflog
echo "" >> $difflog
cat $mylog >> $difflog
echo "" >> $difflog
echo "----------Teacher result--------" >> $difflog
echo "" >> $difflog
cat $teacherlog >> $difflog
echo "" >> $difflog
echo "----------Diff log--------------" >> $difflog
echo "" >> $difflog
diff $mylog $teacherlog >> $difflog

if [ $? -eq 0 ]; then
      clear
      echo "There's no difference, you are good to go"
      cowsay -e "^^" "Good job"

      my_time=$(($my_end-$my_start))
      teacher_time=$(($teacher_end-$teacher_start))
      diff_time=$(($teacher_time - $my_time))

      echo "Your time:       $my_time s"
      echo "Teacher time:    $teacher_time s"
      echo "Time difference: $diff_time s"
else
      clear
      cowsay -d "I don't think you have it good enough. Here's what difference. Remember you can always check the diff log at $difflog"
      cat $difflog | less
fi
