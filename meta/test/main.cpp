#include <iostream>
#include <xaml/meta/meta.hpp>

using namespace std;
using namespace xaml;

class calculator : public meta_class_impl<calculator>
{
public:
    static constexpr std::string_view class_name = "calculator";

    static constexpr std::string_view plus_name = "plus";
    int plus(int x, int y) { return x + y; }
    static constexpr std::string_view minus_name = "minus";
    int minus(int x, int y) { return x - y; }

    ~calculator() override {}

    static void register_class() noexcept
    {
        register_type<calculator>(class_name);
        add_constructor<calculator>();
        add_method<calculator>(plus_name, &calculator::plus);
        add_method<calculator>(minus_name, &calculator::minus);
    }
};

int main()
{
    register_class<calculator>();

    auto mc = construct(*get_type_index(calculator::class_name));
    cout << *invoke_method<int, int, int>(mc, calculator::plus_name, 1, 1) << endl;
    cout << *invoke_method<int, int, int>(mc, calculator::minus_name, 1, 1) << endl;
}
