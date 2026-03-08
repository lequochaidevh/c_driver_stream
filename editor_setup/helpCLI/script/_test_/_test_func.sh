#!/bin/bash -e

cd ../realpath

. "$(dirname "$0")/lib.sh"

export PROJECT_TYPE="TEST FUNCTION"

source setup_enviroment.sh

file_found=$(find_once_file_contain "lib")
echo $file_found