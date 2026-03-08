#!/bin/bash

UTILS_PATH=$(pwd)
DOCS_DIR=$(realpath "$UTILS_PATH")
# Define the list of files
FILES_TO_LINK=("catcmdhelp" "batcmdhelp" "adddocshelp")

cd ../../

. "$(dirname "$0")/lib.sh"

export PROJECT_TYPE="TEST FUNCTION"

source setup_enviroment.sh

# export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export DOCS_DIR=$(realpath "$SCRIPT_DIR/../../docs")

# create file log tag
echo "$DOCS_DIR" > docs_dir.txt
sudo ln -sfv $(pwd)/docs_dir.txt /usr/local/bin/

# Loop through the array
for file in "${FILES_TO_LINK[@]}"; do
    # Get the absolute path of the source file
    SRC=$(realpath "$UTILS_PATH/$file")
    DEST="/usr/local/bin/$file"
    
    # -s: symbolic, -f: force (overwrite), -v: verbose
    sudo ln -sfv "$SRC" "$DEST"

    # Create an absolute path symlink to /usr/local/bin
    LOG_INFO "install symlink $SRC to $DEST"

    if [ -L "/usr/local/bin/batcmdhelp" ]; then
        echo "SUCCESS: batcmdhelp is ready to use."
    fi
done