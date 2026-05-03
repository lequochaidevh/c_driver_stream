## Quick start with stb_truetype.h
**(header only version)**

### Set up format:
-> Get any format for charactor at `/usr/share/fonts/truetype/`
```sh
cp /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf .
# And config file name at main.cpp (line 85)
```

### Build
```sh
g++ -o test main.cpp
# Output: test
```
### Run
```sh
./test
# Output: output.ppm
```

### View image by `feh`
```sh
feh feh output.ppm
```
