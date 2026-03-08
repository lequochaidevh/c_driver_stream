#!/bin/bash

. "$(dirname "$0")/lib.sh"

if [ ! -z "$RESET_ENV_EXIST" ]; then
    # TODO add feature reset set_env.sh
    echo "Reset env"
    echo "$RESET_ENV_EXIST"
elif [ ! -z "$SET_ENV_EXIST" ]; then
    # echo "set_env have been exist"
    return 0
else
    SET_ENV_EXIST="EXISTED"
fi

# This script sets up the environment for the project.
export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ -z "$PROJECT_TYPE" ]; then
    LOG_ERROR "Please export PROJECT_TYPE before running"
    exit 0
fi

if [ "$PROJECT_TYPE" == "ViSolEngine" ]; then
    export WORKSPACE_DIR=$(realpath "$SCRIPT_DIR/../..")
    export CMAKE_ROOT_DIR=$WORKSPACE_DIR
    export CMAKE_BUILD_DIR=$WORKSPACE_DIR/build
    export CMAKE_INSTALL_DIR=$WORKSPACE_DIR/install
    
    # INFO WITHOUT TAG
    LOG_DEBUG "-nt" "%-10sSets up the environment for the ViSolEngine project."

    printf_var "SCRIPT_DIR"
    printf_var "WORKSPACE_DIR"
    printf_var "CMAKE_ROOT_DIR"
    printf_var "CMAKE_BUILD_DIR"
    printf_var "CMAKE_INSTALL_DIR"
elif [ "$PROJECT_TYPE" == "OpenGL_helper" ]; then
    export WORKSPACE_DIR=$(realpath "$SCRIPT_DIR/../..")
    export CMAKE_ROOT_DIR=$WORKSPACE_DIR
    export CMAKE_BUILD_DIR=$WORKSPACE_DIR/build
    export CMAKE_INSTALL_DIR=$WORKSPACE_DIR/install
    
    # INFO WITHOUT TAG
    LOG_DEBUG "-nt" "%-10sSets up the environment for the ViSolEngine project."

    printf_var "SCRIPT_DIR"
    printf_var "WORKSPACE_DIR"
    printf_var "CMAKE_ROOT_DIR"
    printf_var "CMAKE_BUILD_DIR"
    printf_var "CMAKE_INSTALL_DIR"
fi

