#!/bin/bash

if [ $# -gt 1 ]; then
      echo 'parsing wrong'
      exit 0
fi

temp=$1
if [ $# -eq 1 ]; then 
      if [ $temp != 'false' ] && [ $temp != 'true' ]; then
            echo 'argument is "false" and "true" only'
            exit 0
      fi
fi

# remove the previous version
# and prepare for new version
rm -rf dsa171a2
mkdir -p out

# set var to loop through
src=(dbLib.cpp processData.cpp requestLib.cpp)
inc=(dbLib.h dsaLib.h requestLib.h)

# resolve include path
for i in ${src[*]}; do
      echo 'get ' $i '.cpp'
      sed s/\<dsaLib.h\>/\"dsaLib.h\"/ src/"$i" | \
      sed s/\<dbLib.h\>/\"dbLib.h\"/ | \
      sed s/\<requestLib.h\>/\"requestLib.h\"/ > out/"$i"
done

for i in ${inc[*]}; do
      echo 'get ' $i '.h'
      sed s/\<dsaLib.h\>/\"dsaLib.h\"/ include/"$i" | \
      sed s/\<dbLib.h\>/\"dbLib.h\"/ | \
      sed s/\<requestLib.h\>/\"requestLib.h\"/ > out/"$i"
done

echo 'zipping files'
cd out
zip ../src.zip *
cd ..
echo 'zipping done'

# not delete temp folder
if [ $# -eq 1 ] && [ $temp == 'false' ]; then
      echo 'temp folder is kept'
      echo 'Job completed'
      exit 0
fi

# else
echo 'deleting temp folder'
echo 'use "./make-compress.sh false" to keep temp folder'
rm -rf out
echo 'Job completed'
exit 0