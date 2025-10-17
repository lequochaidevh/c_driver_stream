Prebuild:
g++ -std=c++17 -O2 -c exprtk_wrapper.cpp -o exprtk_wrapper.o
BUILD:
g++ plot_function.cpp exprtk_wrapper.o -std=c++17 -O2 \
    -lboost_iostreams -lboost_system -lboost_filesystem -o plot_function