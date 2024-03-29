#!/bin/bash

pwd=$(pwd)  # get file path

############BUILD####################
cowsay "Building...."
cd $pwd/build
make clean
cmake .. -DCMAKE_BUILD_TYPE=Release -DUNIT_TEST=OFF 1> /dev/null
make 1> /dev/null
failed=0
if [ $? -ne 0 ]; then
      # because we want to change cmake back to debug mode
      failed=1
fi
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUNIT_TEST=ON 2> /dev/null 1> /dev/null
cd $pwd

if [ $failed -eq 1 ]; then
      cowsay -d "Sorry, build's Failed"
      exit 1
fi


###############ARGUMENT PARSING####################
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

############### VARIABLES INIT #######################
myapp=$pwd'/bin/dsa171a2'
teacherapp=$pwd'/testapp/dsa171a2'

if [ ! -f $teacherapp ]; then
      cowsay -d "Teacher's testapp not found, atempt to download the file"
      curl "https://github.com/nganhkhoa/CTDL-ASS2/raw/master/testapp/dsa171a2"
      chmod +x $teacherfile
fi

mylog=$pwd'/testapp/my.log'
teacherlog=$pwd'/testapp/teacher.log'
mkdir -p $pwd/testapp/diff
difflog=$pwd'/testapp/diff/'$(date +"%Y-%m-%d_%H:%M:%S")'.log'


################## MY APP ###########################
cowsay "Running my app...."
my_start=$(date +%s)
$myapp $request $data > $mylog
if [ $? -ne 0 ]; then
      cowsay -d "My app run failed, I should have check it"
      rm -rf $mylog $teacherlog
      exit 1
fi
my_end=$(date +%s)


################# TEACHER's APP ##########################
cowsay "Running teacher's app...."
teacher_start=$(date +%s)
$teacherapp $request $data 2> /dev/null 1> $teacherlog
if [ $? -ne 0 ]; then
      cowsay -d "Teacher's app run failed. Aha, teacher has a bug"
      rm -rf $mylog $teacherlog
      exit 1
fi
teacher_end=$(date +%s)


################# DIFF FILE LOG #######################
echo "----------Request file----------" > $difflog
echo "" >> $difflog
cat $request >> $difflog
echo "" >> $difflog
echo "----------My result-------------" >> $difflog
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


############## END #################
if [ $? -eq 0 ]; then
      # if no difference
      clear
      rm -rf $mylog $teacherlog
      echo "There's no difference, you are good to go"
      cowsay -e "^^" "Good job"

      my_time=$(($my_end-$my_start))
      teacher_time=$(($teacher_end-$teacher_start))
      diff_time=$(($teacher_time - $my_time))

      echo "Your time:       $my_time s"
      echo "Teacher time:    $teacher_time s"
      echo "Time difference: $diff_time s"
else
      # there's at least a difference
      clear
      rm -rf $mylog $teacherlog
      cowsay -d "I don't think you have it good enough. Here's what difference. Remember you can always check the diff log at $difflog"
      cat $difflog | less
fi
