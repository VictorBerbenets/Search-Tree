#ifndef GRAPH_TREE
#define GRAPH_TREE

#include <fstream>
#include <filesystem>

namespace graphics {

template<typename PtrType>
class tree_painter {
    using size_type  = std::size_t;
    using value_type = std::remove_pointer_t<PtrType>;
    
    const char* dir_name = "data";

void graph_print() {

}

void create_directory() {
    using namespace std::filesystem;

    std::string abs_name = absolute(__FILE__);
   // path::append()
    directory_entry resource_dir(absolute);

    if (!resource_dir.is_directory()) {
        create_directory(dirs::resource_dir);
    }
}

template<typename... Args>
void graph_print(Args&&... args) {
        
}

public:
    tree_painter(const PtrType ptr) noexcept
    : root_ptr_ {ptr} {}

    void graph_dump(const std::string& file_name = "graph.png") const {
        std::string graph_path = "../";
        std::ofstream graph_file {file_name};
        const char dot_header[] = "digraph List {\n"
                                  "\tdpi = 100;\n"
                                  "\tfontname = \"Comic Sans MS\";\n"
                                  "\tfontsize = 20;\n"
                                  "\trankdir  = TB;\n";
    }

private:
    PtrType root_ptr_;
};

} // <--- namespace graphics

#endif

