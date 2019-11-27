name=$(basename $1)
rm -rf coverage/$name
mkdir -p coverage/$name
cd coverage/$name
find ../../CMakeFiles/$name.dir -name "*.gcno" -exec gcov {} \;
