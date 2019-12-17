#include <iostream>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/meta_macro.hpp>

using namespace std;
using namespace xaml;

class calculator : public meta_class_impl<calculator>
{
public:
    static constexpr std::string_view class_name = "calculator";

    PROP_EVENT(value, int)

public:
    void plus(int x, int y) { set_value(x + y); }
    void minus(int x, int y) { set_value(x - y); }

    ~calculator() override {}

    static void register_class() noexcept
    {
        REGISTER_TYPE();
        ADD_CTOR();
        ADD_METHOD(plus);
        ADD_METHOD(minus);
        ADD_PROP_EVENT(value);
    }
};

int main()
{
    register_class<calculator>();

    auto t = *get_type_index("calculator");
    auto mc = construct(t);
    auto ev = get_event<calculator const&, int>(t, "value_changed");
    auto token = ev.add(mc, function<void(calculator const&, int)>([](calculator const&, int i) { cout << "Value changed: " << i << endl; }));
    invoke_method<void>(mc, "plus", 1, 1);
    ev.remove(mc, token);
    invoke_method<void>(mc, "minus", 1, 1);
}
