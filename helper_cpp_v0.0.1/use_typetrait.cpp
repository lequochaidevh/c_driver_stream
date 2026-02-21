#include <iostream>
#include <assert.h>  // C
#include "helper_type_trait.h"

int main() {
    static_assert(helper::is_vector_v<std::vector<int>>);
    static_assert(helper::is_shared_ptr_v<std::shared_ptr<int>>);
    static_assert(helper::is_string_v < std::string);

    // static_assert(helper::is_string_v<int>);  // assert when compile time

    assert(helper::is_string_v<int> || "Stactic assert");
    assert(helper::is_string_v<int> && "Stactic assert");  // failed affter run

    std::cout << "Hello VN" << std::endl;

    return 0;
}