
set -e

cd ./core

g++ -fPIC -shared \
    -std=c++17 -lstdc++ \
    pipeline_core.cpp \
    plugin_registry.cpp \
    pipeline.cpp \
    pipeline_builder.cpp \
    component/queue/queue_pad.cpp \
    -lyaml-cpp \
    -lpthread \
    -o libpipeline_core.so

cd -

cd ./plugins

g++ -fPIC -shared invert_filter.cpp \
    -I../core \
    -I../core/component \
    -I../core/component/queue \
    -I../core/unit \
    -L../core -lpipeline_core \
    -o libinvert_filter.so

g++ -fPIC -shared sink.cpp \
    -I../core \
    -I../core/component \
    -I../core/component/queue \
    -I../core/unit \
    -L../core -lpipeline_core \
    -o lib_sink.so

g++ -fPIC -shared source.cpp \
    -I../core \
    -I../core/component \
    -I../core/component/queue \
    -I../core/unit \
    -L../core -lpipeline_core \
    -o lib_source.so

cd -

cd ./app

g++ main.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -lyaml-cpp \
    -ldl \
    -o appExc

# LD_LIBRARY_PATH=../core ./appExc

LD_LIBRARY_PATH=../core valgrind --leak-check=full --show-leak-kinds=all ./appExc