#ifndef GRAPH_TREE
#define GRAPH_TREE

#include <fstream>
#include <filesystem>
#include <queue>

#include "node.hpp"

namespace graphics {

template<typename NodeKeyT>
class tree_painter {
    using size_type  = std::size_t;
    using pointer    = yLAB::Node<NodeKeyT>*;
    
    enum class Nodes : char { Left = 1, Right = 2 };

    const std::string dir_name = "data";

std::string create_directory() {
    using namespace std::filesystem;

    std::string curr_path = current_path();
    directory_entry resource_dir(curr_path.append(dir_name));

    if (!resource_dir.is_directory()) {
        create_directory(curr_path.append(1, '/'));
    }
    return curr_path;
}

    void graph_print() {

    }

    template<typename... Args>
    void graph_print(std::ofstream& graph_file, Args&&... args) {
        
    }

    void set_graph(std::ofstream& graph_file, const char* flcol = "lightgreen", float rksep = 1.3,
                   float nodesep = 0.5, const char* style = "rounded", const char* color = "green", float pnwidth = 2.0) const {
        graph_file << "graph [fillcolor = " << flcol << ", ranksep = " << rksep <<
                   ", nodesep = " << nodesep << ", style = \"" << style << "\", color = " <<
                   color << ", penwidth = " << pnwidth << "];\n";
    }
    
    void set_edge(std::ofstream& graph_file, const char* color = "black", const char* arrowhead = "diamond",
                  float arrow_sz = 1.0, float pnwidth = 1.2) const {
        graph_file << "edge [color = " << color << ", arrowhead = " << arrowhead << ", arrowsize = "
                   << arrow_sz << ", penwidth = " << pnwidth << "];\n";
    }

    void create_node(std::ofstream& graph_file, pointer elem, size_type curr_node) const {
        graph_file << "node" << curr_node << " [shape = Mrecord, style = filled, fillcolor = \"#FFD0D0\", label ="
                    "\"{address: "<< elem << "|key: "<< elem.key_ << "|height: "<< elem.height_ <<
                    "| { <ptr1> left: "<< elem->left_ << "| <ptr2> right: " << elem->right_ << "|parent: " << elem->parent_ << "}}\"]\n";
    }

    void create_next_graph_node(std::ofstream& graph_file, pointer elem, size_type curr_node,
                                size_type& next_node, Nodes orientir) const {
        if (!elem) { return ; }
        create_node(graph_file, elem, next_node);
        graph_file << "node" << curr_node << ": <" << (orientir == Nodes::Left ? "ptr1" : "ptr2") << "> -> node"
                   << next_node << "; ";
        ++next_node;
    }

public:
    tree_painter(const pointer ptr)
    : root_ptr_ {ptr},
      dir_path_ {create_directory()} {}

    void graph_dump(const std::string& file_name = "graph") const {
        std::string dot_name = file_name;
        dot_name.append(".dot");
        std::ofstream graph_file {dir_path_ + dot_name};

        const char* dot_header = "digraph List {\n"
                                 "\tdpi = 100;\n"
                                 "\tfontname = \"Comic Sans MS\";\n"
                                 "\tfontsize = 20;\n"
                                 "\trankdir  = TB;\n";
        graph_file << dot_header;

        set_graph(graph_file);
        set_edge(graph_file);

        std::queue<pointer> nodes_queue {};
        nodes_queue.push(root_ptr_);
        
        size_type curr_node {0};
        size_type next_node {0};
        while(nodes_queue.size()) {
            auto elem = nodes_queue.pop();
            create_node(graph_file, elem, curr_node);
            if (elem->left_) {
                create_next_graph_node(graph_file, elem, curr_node, next_node, Nodes::Left);
                nodes_queue.push(elem->left_);
            }
            if (elem->right_) {
                create_next_graph_node(graph_file, elem, curr_node, next_node, Nodes::Right);
                nodes_queue.push(elem->right_);
            }
            graph_file << "\n";
            curr_node++;
        }
        graph_file << "}\n";
    }

private:
    pointer root_ptr_;
    std::string dir_path_;
};

} // <--- namespace graphics

#endif

