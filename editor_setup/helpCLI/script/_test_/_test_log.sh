#!/bin/bash -e

cd ../realpath

. "$(dirname "$0")/lib.sh"

export PROJECT_TYPE="TEST LOG"

source setup_enviroment.sh

LOG_DEBUG "LOG TESTING"
LOG_INFO "LOG TESTING"
LOG_WARN "LOG TESTING"
LOG_ERROR "LOG TESTING"
LOG_SUCCESS "LOG TESTING"
LOG_INFO #error
LOG_INFO "LOG TESTING"