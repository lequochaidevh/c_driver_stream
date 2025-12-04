#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <boost/variant2/variant.hpp>

auto S = [](const char* s) { return std::string(s); };

// using PropValue2 = std::variant<int, double, bool, std::string>;
using PropValue = boost::variant2::variant<
    std::string, int, double, bool
>;
struct Property {
    std::string name;
    PropValue   value;
};

struct MetaNode {
    std::string           name;
    std::vector<Property> props;

    // Type A (copy)
    std::vector<MetaNode> children_copy;

    // Type B: pointer/reference
    std::vector<MetaNode*> children_ptr;

    // For pointer tree
    void add_child_ptr(MetaNode* child) { children_ptr.push_back(child); }

    // For copy tree
    MetaNode& add_child_copy(const MetaNode& child) {
        children_copy.push_back(child);
        return children_copy.back();
    }
};

struct RegisterToParent {
    RegisterToParent(MetaNode& parent, MetaNode& child) { parent.add_child_ptr(&child); }
};

/*
EO
 ├── module_OV9782@0
 |    |
 │    │── FOV
 │    │    │────── horizontal
 │    |    │           └────── <value>
 │    |    │
 |    |    │────── vertical
 |    |    |           └────── <value>
 │    |    │
 |    |    └───────── ELF
 |    |                └────── <value>
 │    │
 │    │── mode_a
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    │── mode_b
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    └─ stream
 │        └────── keyobc
 │                    |────── orange_pi_5 <value>
 │                    |
 |                    └────── raspberry_pi_6 <value>
 |
 ├── module_OV9782@1
 |    |
 │    │── FOV
 │    │    │────── horizontal
 │    |    │           └────── <value>
 │    |    │
 |    |    │────── vertical
 |    |    |           └────── <value>
 │    |    │
 |    |    └───────── ELF
 |    |                └────── <value>
 │    │
 │    │── mode_a
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    │── mode_b
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    └─ stream
 │        └────── keyobc
 │                    |────── orange_pi_5 <value>
 │                    |
 |                    └────── raspberry_pi_6 <value>
 │
 ├── module_ov7251@0
 |    |
 │    │── FOV
 │    │    │────── horizontal
 │    |    │           └────── <value>
 │    |    │
 |    |    │────── vertical
 |    |    |           └────── <value>
 │    |    │
 |    |    └───────── ELF
 |    |                └────── <value>
 │    │
 │    │── mode_a
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    │── mode_b
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    └─ stream
 │        └────── keyobc
 │                    |────── orange_pi_5 <value>
 │                    |
 |                    └────── raspberry_pi_6 <value>
 ├── module_ov7251@0
 |    |
 │    │── FOV
 │    │    │────── horizontal
 │    |    │           └────── <value>
 │    |    │
 |    |    │────── vertical
 |    |    |           └────── <value>
 │    |    │
 |    |    └───────── ELF
 |    |                └────── <value>
 │    │
 │    │── mode_a
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    │── mode_b
 │    │    └────── keyobc
 │    |               |────── orange_pi_5 <value>
 │    |               |
 |    |               └────── raspberry_pi_6 <value>
 │    └─ stream
 │        └────── keyobc
 │                    |────── orange_pi_5 <value>
 │                    |
 |                    └────── raspberry_pi_6 <value>
*/

MetaNode metadata = {"EO",
                     {},
                     {

                         // ================= module_OV9782@0 =================
                         {"module_OV9782@0",
                          {},
                          {

                              {"FOV",
                               {},
                               {
                                   {"horizontal", {{"value", 97.34}}, {}},
                                   {"vertical", {{"value", 58.29}}, {}},
                                   {"ELF", {{"value", 3.41}}, {}},
                               }},

                              {"mode_a",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Q7f9Kd2A")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("cP1aLx8T")}}, {}}
                                    }},
                               }},

                              {"mode_b",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Jr8Qe2Pw")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Ks4Mn1Zh")}}, {}}
                                    }},
                               }},

                              {"stream",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Ab9Dk73R")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Lp2Xe90M")}}, {}}
                                    }},
                               }},
                          }},

                         // ================= module_OV9782@1 =================
                         {"module_OV9782@1",
                          {},
                          {

                              {"FOV",
                               {},
                               {
                                   {"horizontal", {{"value", 88.12}}, {}},
                                   {"vertical", {{"value", 63.77}}, {}},
                                   {"ELF", {{"value", 1.94}}, {}},
                               }},

                              {"mode_a",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Xe1Tq77K")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Ua9Lm4Bd")}}, {}}
                                    }},
                               }},

                              {"mode_b",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Nt4Az3Rp")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Gk6Pq8Hz")}}, {}}
                                    }},
                               }},

                              {"stream",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Vr2Dk66N")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Qp8Hs2Fj")}}, {}}
                                    }},
                               }},
                          }},

                         // ================= module_ov7251@0 =================
                         {"module_ov7251@0",
                          {},
                          {

                              {"FOV",
                               {},
                               {
                                   {"horizontal", {{"value", 112.45}}, {}},
                                   {"vertical", {{"value", 74.53}}, {}},
                                   {"ELF", {{"value", 4.12}}, {}},
                               }},

                              {"mode_a",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Tz4Qm7Lf")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Aa2Bf91D")}}, {}}
                                    }},
                               }},

                              {"mode_b",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Wm3Ef6Po")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Dn8Qx2Sv")}}, {}}
                                    }},
                               }},

                              {"stream",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Hv9Jt21K")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Qo6Lp4Sa")}}, {}}
                                    }},
                               }},
                          }},

                         // ================= module_ov7251@1 =================
                         {"module_ov7251@1",
                          {},
                          {

                              {"FOV",
                               {},
                               {
                                   {"horizontal", {{"value", 105.88}}, {}},
                                   {"vertical", {{"value", 69.12}}, {}},
                                   {"ELF", {{"value", 2.61}}, {}},
                               }},

                              {"mode_a",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Hr5Qc79E")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Sp3Vx10K")}}, {}}
                                    }},
                               }},

                              {"mode_b",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value", std::string("Mx9Bk2Vd")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Lq1Np6Ys")}}, {}}
                                    }},
                               }},

                              {"stream",
                               {},
                               {
                                   {"keyobc",
                                    {},
                                    {
                                        {"orange_pi_5", {{"value",  S("Za4Kd85Q")}}, {}},
                                        {"raspberry_pi_6", {{"value",  S("Tx7Pf39W")}}, {}}
                                    }},
                               }},
                          }}}};

MetaNode* find_node(MetaNode& node, const std::string& path) {
    if (path == node.name) return &node;

    for (auto& child : node.children_copy) {
        if (child.name == path) return &child;

        if (auto* found = find_node(child, path)) return found;
    }
    return nullptr;
}

Property* get_prop(MetaNode& node, const std::string& key) {
    for (auto& p : node.props)
        if (p.name == key) return &p;
    return nullptr;
}

void print_prop(const Property& p) {
    std::cout << p.name << " = ";
    boost::variant2::visit([](auto&& v) { std::cout << v; }, p.value);
    std::cout << "\n";
}

MetaNode* find_path(MetaNode& root, const std::string& path) {
    std::stringstream ss(path);
    std::string       item;

    MetaNode* current = &root;

    while (std::getline(ss, item, '/')) {
        bool found = false;
        for (auto& child : current->children_copy) {
            if (child.name == item) {
                current = &child;
                found   = true;
                break;
            }
        }
        if (!found) return nullptr;
    }
    return current;
}

Property* get_prop_path(MetaNode& root, const std::string& path, const std::string& prop_name) {
    MetaNode* node = find_path(root, path);
    if (!node) return nullptr;

    for (auto& p : node->props) {
        if (p.name == prop_name) return &p;
    }
    return nullptr;
}

int main() {
    auto* prop = get_prop_path(metadata, "module_OV9782@0/FOV/horizontal", "value");

    if (!prop) return 0;

    boost::variant2::visit([](auto&& v) { std::cout << "Value = " << v << "\n"; }, prop->value);

    auto* prop2 = get_prop_path(metadata, "module_ov7251@1/mode_b/keyobc/raspberry_pi_6", "value");

    if (!prop2) return 0;

    boost::variant2::visit([](auto&& v) { std::cout << "Fake keyobc is = " << v << "\n"; }, prop2->value);

}