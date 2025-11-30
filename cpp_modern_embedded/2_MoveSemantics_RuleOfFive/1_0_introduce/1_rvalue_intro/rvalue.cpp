#include <iostream>
#include <string>

std::string make() {
    std::string make_str = "ABCDEFGHJKLMNOPQ";
    return make_str;
}

int main() {
    std::string str1 = make();  // rvalue at here
    std::string str2;
    str2 = make();
    std::cout << "\n";
}




