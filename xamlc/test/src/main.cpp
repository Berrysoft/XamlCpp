#include <iostream>
#include <xaml/compiler.hpp>
#include <xaml/parser.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

int main()
{
    init_parser();
    add_compiler_module("xaml_ui_controls_meta");
    string_view file = "test.xaml";
    parser p{ file };
    if (p.is_open())
    {
        compiler c{};
        xaml_node node = p.parse();
        c.compile(node, cout) << endl;
    }
}
