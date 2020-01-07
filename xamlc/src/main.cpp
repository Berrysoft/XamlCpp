#include <iostream>
#include <xaml/compiler.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

int main()
{
    init_parser();
    string_view file = "test.xaml";
    compiler c{ file };
    if (c.is_open())
    {
        c.compile(cout) << endl;
    }
}
