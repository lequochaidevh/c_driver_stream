#include <iostream>
#include <string>

class Bar {
public:
    std::string _name;
    Bar(std::string name) : _name(name) {
        std::cout << "Bar " << _name << " constructor called\n";
    }
    ~Bar() {
        std::cout << "Bar " << _name << " destructor called\n";
    }
};

void logBar(Bar& bar) {
    std::cout << "Bar " << bar._name << " get by Bar& \n";
}

void logBar(Bar&& bar) { // 
    std::cout << "Bar " << bar._name << " get by Bar&& \n";
}

//
Bar lvalue("lvalue");

int main() {
    std::cout << "----- main() start scope\n";
    logBar(Bar("rvalue")); // arg = "temporary value" ~(prvalue) Distructor after out of logBar()

    logBar(lvalue); // arg = "lvalue" Distructor after out of main()
    std::cout << "----- main() stop scope\n";
}

/*
result:
devh### g++ scope.cpp -o scope 
devh### ./scope 
Bar lvalue constructor called
----- main() start scope
Bar rvalue constructor called
Bar rvalue get by Bar&& 
Bar rvalue destructor called
Bar lvalue get by Bar& 
----- main() stop scope
Bar lvalue destructor called

*/