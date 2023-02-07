source ~/.zprofile

echo building "$1"
FILE_NAME=$1
shift
rm -rf build
mkdir build
cd build

COMPILE_CMD='echo compile command not set'

COMPILE_CMD_PREFIX='g++12'
COMPILE_CMD=' -o main ../${FILE_NAME} -std=c++20 -O3 -ftrack-macro-expansion=0 -fpermissive -g'

# Parse arguments
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
    --use_boost)
        COMPILE_CMD+=' -I ${BOOST_INCLUDEDIR} -L ${BOOST_LIBRARYDIR}'
        shift
        ;;
    --big_stack)
        COMPILE_CMD+=' -Wl,-stack_size,0x10000000,-stack_addr,0xc0000000'
        shift
        ;;
    --defines)
        shift
        other_args="$*"
        COMPILE_CMD_PREFIX+=" "
        COMPILE_CMD_PREFIX+=$other_args
        break
        ;;
    *)
        echo "cannot match argument: $key"
        exit
        ;;
    esac
done

COMPILE_CMD="${COMPILE_CMD_PREFIX}${COMPILE_CMD}"
echo "with command:\n\t"$COMPILE_CMD"\n"

eval $COMPILE_CMD
echo "\n************Program Output************\n"
./main
echo "\n**************************************\n"