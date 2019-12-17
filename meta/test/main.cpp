#include <iostream>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/meta_macro.hpp>

using namespace std;
using namespace xaml;

class calculator : public meta_class_impl<calculator>
{
public:
    static constexpr std::string_view class_name = "calculator";

    int plus(int x, int y) { return x + y; }
    int minus(int x, int y) { return x - y; }

    ~calculator() override {}

    static void register_class() noexcept
    {
        REGISTER_TYPE();
        ADD_CTOR();
        ADD_METHOD(plus);
        ADD_METHOD(minus);
    }
};

int main()
{
    register_class<calculator>();

    auto mc = construct(*get_type_index("calculator"));
    cout << *invoke_method<int>(mc, "plus", 1, 1) << endl;
    cout << *invoke_method<int>(mc, "minus", 1, 1) << endl;
}
