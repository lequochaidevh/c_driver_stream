#!/usr/bin/bash


# g++ gen_auto_complete.cpp -o gen_auto_complete -std=c++17
# ./gen_auto_complete cli2.json > auto_complete.sh
# source auto_complete.sh
g++ gen_auto_complete.cpp -std=c++17 -o gen_auto_complete
./gen_auto_complete cli_tree.json > auto_complete.sh
bash -n auto_complete.sh
source auto_complete.sh
