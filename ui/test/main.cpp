#include <iostream>
#include <xaml/ui/event.hpp>
#include <xaml/ui/meta.hpp>
#include <xaml/ui/meta_macro.hpp>

using namespace std;
using namespace xaml;

class storage : public meta_class_impl<storage>
{
public:
    static constexpr std::string_view class_name = "storage";

    PROP_EVENT(value, int)

    ~storage() override {}

    static void register_class() noexcept
    {
        REGISTER_TYPE();
        ADD_CTOR();
        ADD_PROP_EVENT(value);
    }
};

int main()
{
    register_class<storage>();

    auto t = *get_type_index("storage");
    auto mc = construct(t);
    auto ev = get_event<storage const&, int>(t, "value_changed");
    auto token = ev.add(mc, function<void(storage const&, int)>([](storage const&, int i) { cout << "Value changed: " << i << endl; }));
    auto prop = get_property(t, "value");
    prop.set(mc, 100);
    ev.invoke<storage const&, int>(mc, *reinterpret_pointer_cast<storage>(mc), 150);
    ev.remove(mc, token);
    prop.set(mc, 200);
}
