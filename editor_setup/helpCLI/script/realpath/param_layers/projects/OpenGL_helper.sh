#!/bin/bash

while [[ $# -gt 0 ]]; do
    case "$1" in
        --project|-p)
            # Push variable to CMAKE
            export SRC_NAME=$2
            echo "Cmake build file: $SRC_NAME"
            shift
            shift
            ;;
        --clear|--clean|-c)
            echo "Action: RM_BUILD=1 ..."
            RM_BUILD=1
            shift
            ;;
        --help|-h)
            echo "Usage: ./build.sh "
            echo "-p [file.cpp]"
            echo "-c (or --clean|--clear) to remove build after run"
            exit 0
            ;;
        *)
            OTHER_ARGS+=("$1")
            shift
            ;;
    esac
done