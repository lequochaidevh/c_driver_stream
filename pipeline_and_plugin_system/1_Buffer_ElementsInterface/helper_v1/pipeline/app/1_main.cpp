#include "../core/pipeline_core.hpp"
#include <dlfcn.h>
#include <iostream>

int main() {
    void* handle = dlopen("../plugins/libinvert_filter.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << dlerror() << std::endl;
        return -1;
    }

    using CreateFn  = Element* (*)();
    CreateFn create = (CreateFn)dlsym(handle, "create");

    if (!create) {
        std::cerr << dlerror() << std::endl;
        return -1;
    }

    Element* filter = create();
    filter->init();

    BufferPtr in = std::make_shared<Buffer>();
    in->data.resize(10, 100);

    BufferPtr out;
    filter->process(in, out);

    std::cout << "Processed buffer size=" << out->data.size() << std::endl;

    filter->shutdown();
    delete filter;
    dlclose(handle);
}

/*
g++ main.cpp \
    -I../core \
    -L../core -lpipeline_core \
    -ldl \
    -o app

    */