#ifndef GRAPH_TREE
#define GRAPH_TREE

#include <fstream>
#include <filesystem>
#include <concepts>
#include <queue>

#include "node.hpp"

namespace graphics {

template<typename NodeKeyT>
class tree_painter {
    using size_type    = std::size_t;
    using node_type    = yLAB::detail::Node<NodeKeyT>;
    using end_node_ptr = yLAB::detail::EndNode<NodeKeyT>*;
    using pointer      = node_type*;

    enum class Nodes : char { Left = 1, Right = 2 };

    const std::string DIR_NAME = "data";

    std::string create_dir() {
        using namespace std::filesystem;

        std::string relative_path = "../include/";
        directory_entry resource_dir(relative_path.append(DIR_NAME));
        relative_path.append("/");
        if (!resource_dir.is_directory()) {
            create_directory(relative_path);
        }
        return relative_path;
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
        // to do
        if (std::same_as<decltype(elem), const end_node_ptr>) {
            graph_file << "node" << curr_node << " [shape = Mrecord, style = filled, fillcolor = \"#ABFFF1\", label ="
                        "\"{End Node | {address: " << elem << "} | { <ptr1> left: " << elem->left_ <<  "}}\"]\n";
        } else {
            graph_file << "node" << curr_node << " [shape = Mrecord, style = filled, fillcolor = \"#FFD0D0\", label ="
                        "\"{address: "<< elem << "|key: "<< elem->key_ << "|height: "<< elem->height_ << "|size: " << elem->size_ <<
                        "| { <ptr1> left: " << elem->left_ << "| <ptr2> right: " << elem->right_ << "|parent: " << elem->parent_ << "}}\"]\n";
        }
    }

    void create_next_graph_node(std::ofstream& graph_file, pointer elem, size_type curr_node,
                                size_type& next_node, Nodes orientir) const {
        if (!elem) { return ; }
        create_node(graph_file, elem, next_node);
        graph_file << "node" << curr_node << ": <" << (orientir == Nodes::Left ? "ptr1" : "ptr2") << "> -> node"
                   << next_node << "; ";
        ++next_node;
    }

    void create_png_graph(const std::string& dot_name, std::string file_name) const {
        std::string system_command = "dot -Tpng ";
        system_command.append(dir_path_ + dot_name);
        system_command.append(" -o ");
        system_command.append(dir_path_ + file_name.append(".png"));
        std::cout << "command = " << system_command.c_str() << std::endl;

        system(system_command.c_str());
    }

public:
    tree_painter(const pointer ptr)
    : root_ptr_ {ptr},
      dir_path_ {create_dir()} {}

    void graph_dump(const std::string& file_name = "graph") const {
        if (!root_ptr_) {
            std::cout << "Tree is empty" << std::endl;
            return ;
        }
        std::string dot_name = file_name;
        std::string dot_file = dir_path_ +  dot_name.append(".dot");
        std::ofstream graph_file {dot_file};
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

        size_type curr_node {1};
        size_type next_node {2};
        while(nodes_queue.size()) {
            auto elem = nodes_queue.front();
            nodes_queue.pop();
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
        create_png_graph(dot_name, file_name);
    }

private:
    pointer root_ptr_;
    std::string dir_path_;
};

} // <--- namespace graphics

#endif

