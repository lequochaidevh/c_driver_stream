
set -e

cd ./app

g++ main.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -lyaml-cpp \
    -ldl \
    -I../../../third_party/spdlog/include \
    -o appExc

LD_LIBRARY_PATH=../core ./appExc

# LD_LIBRARY_PATH=../core valgrind --leak-check=full --show-leak-kinds=all ./appExc

    # -L../prebuild -lspdlog \