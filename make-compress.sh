#!/bin/bash

# remove the previous version
# and prepare for new version
rm -rf dsa171a2
mkdir -p out

# set var to loop through
src=(dbLib.cpp processData.cpp requestLib.cpp)
inc=(dbLib.h dsaLib.h requestLib.h)

# resolve include path
for i in ${src[*]}
do
      echo 'get ' $i '.cpp'
      cp src/$i out/$i
      sed s/\<dsaLib.h\>/\"dsaLib.h\"/ src/"$i" | \
      sed s/\<dbLib.h\>/\"dbLib.h\"/ | \
      sed s/\<requestLib.h\>/\"requestLib.h\"/ > out/"$i"
done

for i in ${inc[*]}
do
      echo 'get ' $i '.h'
      cp include/$i out/$i
      sed s/\<dsaLib.h\>/\"dsaLib.h\"/ include/"$i" | \
      sed s/\<dbLib.h\>/\"dbLib.h\"/ | \
      sed s/\<requestLib.h\>/\"requestLib.h\"/ > out/"$i"
done

echo 'zipping files'
cd out
zip ../dsa171a2.zip *
cd ..

echo 'delete temp folder'
rm -rf out

echo "Job completed"