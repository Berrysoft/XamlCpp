#include <iostream>
#include <xaml/meta/meta.hpp>

#define REGISTER_TYPE(type) ::xaml::register_type<type>(#type)
#define ADD_CONSTRUCTOR(type, ...) ::xaml::add_constructor<type, ##__VA_ARGS__>()
#define ADD_METHOD(type, name) ::xaml::add_method<type>(#name, &type::name)

using namespace std;
using namespace xaml;

class calculator : public meta_class_impl<calculator>
{
public:
    int add(int x, int y) { return x + y; }
    int minus(int x, int y) { return x - y; }

    ~calculator() override {}

    static void register_class() noexcept
    {
        REGISTER_TYPE(calculator);
        ADD_CONSTRUCTOR(calculator);
        ADD_METHOD(calculator, add);
        ADD_METHOD(calculator, minus);
    }
};

int main()
{
    register_class<calculator>();

    auto mc = construct(*get_type_index("calculator"));
    cout << *invoke_method<int, int, int>(mc, "add", 1, 1) << endl;
    cout << *invoke_method<int, int, int>(mc, "minus", 1, 1) << endl;
}
