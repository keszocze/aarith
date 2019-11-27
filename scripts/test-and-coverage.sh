BASEDIR=$(dirname "$0")
find -name "*-test" -exec {} \; -exec $BASEDIR/run-coverage.sh {} \;
