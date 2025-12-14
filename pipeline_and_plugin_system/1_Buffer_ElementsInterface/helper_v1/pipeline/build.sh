
set -e

cd ./core

g++ -fPIC -shared \
    -std=c++17 -lstdc++ \
    pipeline_core.cpp \
    plugin_registry.cpp \
    pipeline.cpp \
    pipeline_builder.cpp \
    -I. \
    -lyaml-cpp \
    -o libpipeline_core.so

cd -

cd ./plugins

g++ -fPIC -shared invert_filter.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -o libinvert_filter.so

g++ -fPIC -shared debug_sink.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -o libdebug_sink.so

cd -

cd ./app

g++ main.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -lyaml-cpp \
    -ldl \
    -o appExc

LD_LIBRARY_PATH=../core ./appExc