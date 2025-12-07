/**
 * CHANGELOG
 * Split Parent node and Children node.
 * Declare coppy data and ptr assign
 * */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

#include <algorithm>
#include "/home/devh/localPc_files/linux_std/helper_encrypt_string.h"
#include "meta_data_tree.h"

#include <boost/variant2/variant.hpp>

#define DEBUG_MODE
// variant in c++17 or boost::variant2::variant willl convert const char* (string) to bool.
// declare not convert to bool whenever use string
// #define S(x) std::string(x)
inline std::string S(const char* x) { return std::string(x); }
#define E(str) SENC(ENC(str))
using PropValue = boost::variant2::variant<int, double, bool, std::string>;

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
        // MetaNode& new_child = children_copy.back();
        // children_ptr.push_back(&new_child);
        return children_copy.back();
    }

    // Constructor will NOT custom because it CAN NOT use initializer list
    // Todo update later
};

struct RegisterToParent {
    RegisterToParent(MetaNode* parent, MetaNode* child) {
        std::cout << "[Register ptr] parent=" << parent->name << " (" << parent << ")"
                  << " child=" << child->name << " (" << child << ")\n";
        parent->add_child_ptr(child);
    }
    RegisterToParent(MetaNode& parent, MetaNode& child) {
        std::cout << "[Register cp] parent=" << parent.name << " (" << &parent << ")"
                  << " child=" << child.name << " (" << &child << ")\n";
        parent.add_child_copy(child);
    }
};

MetaNode OV9782module_0 =
    // ================= module_OV9782@0 =================
    {E("module_OV9782@0"),
     {},
     {

         {E("MECHA"),
          {},
          {
              {E("X_LOCAL"), {{E("value"), 97.34}}, {}},
              {E("Y_LOCAL"), {{E("value"), 58.29}}, {}},
              {E("ELF"), {{E("value"), 3.41}}, {}},
          }},

         {E("mode_a"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Q7f9Kd2A")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("cP1aLx8T")}}, {}},
               }},
          }},

         {E("mode_b"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Jr8Qe2Pw")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Ks4Mn1Zh")}}, {}},
               }},
          }},

         {E("stream"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Ab9Dk73R")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Lp2Xe90M")}}, {}},
               }},
          }},
     }};

MetaNode OV9782module_1 =
    // ================= module_OV9782@1 =================
    {E("module_OV9782@1"),
     {},
     {

         {E("MECHA"),
          {},
          {
              {E("X_LOCAL"), {{E("value"), 88.12}}, {}},
              {E("Y_LOCAL"), {{E("value"), 63.77}}, {}},
              {E("ELF"), {{E("value"), 1.94}}, {}},
          }},

         {E("mode_a"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Xe1Tq77K")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Ua9Lm4Bd")}}, {}},
               }},
          }},

         {E("mode_b"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Nt4Az3Rp")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Gk6Pq8Hz")}}, {}},
               }},
          }},

         {E("stream"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Vr2Dk66N")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Qp8Hs2Fj")}}, {}},
               }},
          }},
     }};

MetaNode ov7251module_0 =
    // ================= module_ov7251@0 =================
    {E("module_ov7251@0"),
     {},
     {

         {E("MECHA"),
          {},
          {
              {E("X_LOCAL"), {{E("value"), 112.45}}, {}},
              {E("Y_LOCAL"), {{E("value"), 74.53}}, {}},
              {E("ELF"), {{E("value"), 4.12}}, {}},
          }},

         {E("mode_a"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Tz4Qm7Lf")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Aa2Bf91D")}}, {}},
               }},
          }},

         {E("mode_b"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Wm3Ef6Po")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Dn8Qx2Sv")}}, {}},
               }},
          }},

         {E("stream"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Hv9Jt21K")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Qo6Lp4Sa")}}, {}},
               }},
          }},
     }};

MetaNode ov7251module_1 =
    // ================= module_ov7251@1 =================
    {E("module_ov7251@1"),
     {},
     {

         {E("MECHA"),
          {},
          {
              {E("X_LOCAL"), {{E("value"), 105.88}}, {}},
              {E("Y_LOCAL"), {{E("value"), 69.12}}, {}},
              {E("ELF"), {{E("value"), 2.61}}, {}},
          }},

         {E("mode_a"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Hr5Qc79E")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Sp3Vx10K")}}, {}},
               }},
          }},

         {E("mode_b"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Mx9Bk2Vd")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Lq1Np6Ys")}}, {}},
               }},
          }},

         {E("stream"),
          {},
          {
              {E("keyobc"),
               {},
               {
                   {E("orange_pi_5"), {{E("value"), S("Za4Kd85Q")}}, {}},
                   {E("raspberry_pi_6"), {{E("value"), S("Tx7Pf39W")}}, {}},
               }},
          }},
     }};

MetaNode metadata_EO_cameras =
    // ================= EO_cameras =================
    {E("EO_cameras"),
     {},   // Init without props
     {}};  // Init without children

/* How to attach child Node to parent Node*/
// Method 1: Declare register struct to handle. Can work in global.
// static RegisterToParent register1(metadata_EO_cameras, OV9782module_0);    // COPY + ptr->COPY
// static RegisterToParent register2(&metadata_EO_cameras, &ov7251module_1);  // ptr (ONLY)

// Method 2: Declare register. -> main()
#define USE_ADD_FUNC

MetaNode* find_node(MetaNode& node, const std::string& path) {
    if (path == node.name) return &node;

    for (auto& child : node.children_copy) {
        if (child.name == path) return &child;

        if (auto* found = find_node(child, path)) return found;
    }
    return nullptr;
}

MetaNode* find_node(MetaNode* node, const std::string& path) {
    if (path == node->name) return node;

    for (auto& child : node->children_copy) {
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

MetaNode* find_path(MetaNode* root, const std::string& path) {
    if (!root) return nullptr;
    std::stringstream ss(path);
    std::string       item;
    MetaNode*         current = root;

    while (std::getline(ss, item, '/')) {
        bool found = false;
        // 1) Try children_ptr first (fast)
        for (auto* child : current->children_ptr) {
            if (!child) continue;
            if (child->name == item) {
                current = child;
                found   = true;
                break;
            }
        }
        if (found) continue;

        // 2) Fallback: search children_copy (addresses differ but names may match)
        // for (auto& child_copy : current->children_copy) {
        //     if (child_copy.name == item) {
        //         current = &child_copy;
        //         found   = true;
        //         break;
        //     }
        // }

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

Property* get_prop_path(MetaNode* root, const std::string& path, const std::string& prop_name) {
    MetaNode* node = find_path(root, path);
    if (!node) return nullptr;

    for (auto& p : node->props) {
        if (p.name == prop_name) return &p;
    }
    return nullptr;
}

void print_tree(const MetaNode& node, int indent = 0) {
    std::cout << std::string(indent, ' ') << node.name << "\n";
    std::cout << std::string(indent + 2, ' ') << "children_copy=" << node.children_copy.size()
              << " children_ptr=" << node.children_ptr.size() << "\n";

    for (const auto& c : node.children_copy) print_tree(c, indent + 4);
    for (auto* p : node.children_ptr) {
        if (p)
            std::cout << std::string(indent + 4, ' ') << "[ptr] " << p->name << " (" << p << ")\n";
        else
            std::cout << std::string(indent + 4, ' ') << "[ptr] nullptr\n";
    }
}

void print_tree(const MetaNode* node, int indent = 0) {
    if (!node) return;

    std::cout << std::string(indent, ' ') << node->name << "\n";
    std::cout << std::string(indent + 2, ' ') << "children_copy=" << node->children_copy.size()
              << " children_ptr=" << node->children_ptr.size() << "\n";

    // ===== 1) Prefer children_ptr =====
    if (!node->children_ptr.empty()) {
        for (auto* child : node->children_ptr) {
            print_tree(child, indent + 4);
        }
        return;
    }

    // ===== 2) fallback: children_copy =====
    // for (const auto& child_copy : node->children_copy) {
    //     print_tree(&child_copy, indent + 4);
    // }
}

// Recursively sync children_ptr so pointers always refer to entries in children_copy
void sync_children_ptr_recursive(MetaNode& node, bool re_sync_root = false) {
    static std::vector<MetaNode*> rootNode_children_ptr = {};
    static MetaNode               rootNode;
    if (re_sync_root == true) {
        rootNode_children_ptr = {};
    }
    if (rootNode_children_ptr.empty()) {  // Preprocess recursive INIT
        rootNode_children_ptr = node.children_ptr;
        rootNode              = node;
        node.children_ptr.clear();
    }
    for (auto& child_copy : node.children_copy) {
        node.children_ptr.push_back(&child_copy);
    }
    // Recurse into the copies (important so pointers of deeper levels get set)
    for (auto& child_copy : node.children_copy) {
        sync_children_ptr_recursive(child_copy);
    }
    if (rootNode.name == node.name) {  // Root recursive
        for (auto child_copy_ptr : rootNode_children_ptr) {
            sync_children_ptr_recursive(*child_copy_ptr);
        }
        node.children_ptr.insert(node.children_ptr.end(), rootNode_children_ptr.begin(), rootNode_children_ptr.end());
    }
}
int main_init() {
#ifdef USE_ADD_FUNC
    metadata_EO_cameras.add_child_copy(OV9782module_0);  // OK // COPY + ptr->COPY
    metadata_EO_cameras.add_child_ptr(&ov7251module_1);  // OK // ptr (ONLY)
    sync_children_ptr_recursive(metadata_EO_cameras);
#endif

#ifdef DEBUG_MODE
    std::cout << "&metadata_EO_cameras = ";
    std::cout << &metadata_EO_cameras << "\n";

    std::cout << "&metadata_EO_cameras.children_copy[0] = ";
    std::cout << &metadata_EO_cameras.children_copy[0] << "\n";

    std::cout << "&metadata_EO_cameras.children_copy[1] = ";
    std::cout << &metadata_EO_cameras.children_copy[1] << "\n";  // invalid adresss

    std::cout << "metadata_EO_cameras.children_ptr[0] = ";
    std::cout << metadata_EO_cameras.children_ptr[0] << "\n";

    std::cout << "metadata_EO_cameras.children_ptr[1] = ";
    std::cout << metadata_EO_cameras.children_ptr[1] << "\n";
#endif

    std::string parser_find = E("module_OV9782@0/MECHA/X_LOCAL");
    std::cout << parser_find << "\n";
    auto* prop = get_prop_path(metadata_EO_cameras, parser_find, E("value"));

    if (!prop) return 0;

    boost::variant2::visit([](auto&& v) { std::cout << "Value = " << v << "\n"; }, prop->value);

    prop = get_prop_path(&metadata_EO_cameras, E("module_ov7251@1/mode_b/keyobc/raspberry_pi_6"), E("value"));

    if (!prop) return 0;

    boost::variant2::visit([](auto&& v) { std::cout << "Fake keyobc is = " << v << "\n"; }, prop->value);

#ifdef DEBUG_MODE
    print_tree(&metadata_EO_cameras);
#endif

    return 0;
}

int main() { main_init(); }