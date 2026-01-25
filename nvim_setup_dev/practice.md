### Neotree
space v : open
a : create file
r : rename file
o : revert change file before
f : search file, open and space v
c : cp file
ctrl + alt + <w,h.l> switch neotree and editor

### Insert mode
di" : delete all word in "..."
di< : delete all word in <...>
da" : detele string include "
da" : detele string include "
dt" : delete all word from CURSOR to next "

ctrl+ alt+ s : save file

<C-w>
<C-u>
<C-w>

### Comment code
:%s/^/\/\//
or use plugin
V -> gc

### Remote editor
nvim scp://user@remote-ip//path/to/folder/
**Note**: Need "//"

### Multi insert word in multi line parrallel

**normal**
ctrl + v : visual block
h j k l : choose string
:normal i"yrstring"

or: :s/^/"string"/

delete: norm xx : remove 2 charactor



