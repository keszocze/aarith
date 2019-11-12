name=$(basename $1)
rm -rf coverage/$name
mkdir -p coverage/$name
cd coverage/$name
gcov ../../CMakeFiles/$name.dir/$name.cpp.gcno
