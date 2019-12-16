#include <iostream>
#include <xaml/meta/meta.hpp>

using namespace std;
using namespace xaml;

class my_class : public meta_class_impl<my_class>
{
public:
    int add(int x, int y) { return x + y; }
};

int main()
{
    register_type<my_class>("my_class");
    add_constructor<my_class>();
    add_method<my_class>("add", &my_class::add);

    auto mc = construct(*get_type_index("my_class"));
    cout << *invoke_method<int, int, int>(mc, "add", 1, 1) << endl;
}
