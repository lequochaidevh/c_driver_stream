#!/bin/bash

if [ ! -z "$PROJECT_TYPE" ]; then
    ./param_layers/projects/"$PROJECT_TYPE".sh $@
fi
