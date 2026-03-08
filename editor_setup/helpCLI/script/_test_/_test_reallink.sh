#!/bin/bash -e

cd ../realpath

. "$(dirname "$0")/lib.sh"

export PROJECT_TYPE="TEST REAL PATH"

source setup_enviroment.sh

LOG_DEBUG "${SCRIPT_DIR}"
LOG_DEBUG "${WORKSPACE_DIR}"
