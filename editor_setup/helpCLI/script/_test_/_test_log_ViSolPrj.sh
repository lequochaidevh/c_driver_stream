#!/bin/bash -e

cd ../realpath

. "$(dirname "$0")/lib.sh"


LOG_DEBUG "LOG TESTING"

export PROJECT_TYPE="ViSolEngine"

source setup_enviroment.sh
