#include <iostream>
#include <xaml/compiler.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        init_meta();
        string_view file = argv[1];
        compiler c{ file };
        if (c.is_open())
        {
            c.compile(cout) << endl;
        }
    }
}
