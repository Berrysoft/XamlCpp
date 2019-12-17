#include <iostream>
#include <xaml/ui/event.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

class storage : public meta_class_impl<storage>
{
private:
    int _value;

    event<int> _value_changed;

    void invoke_value_changed(int value) { _value_changed(value); }

public:
    int get_value() const noexcept { return _value; }
    void set_value(int v) noexcept
    {
        if (_value != v)
        {
            _value = v;
            invoke_value_changed(v);
        }
    }

    template <typename F>
    size_t add_value_changed(F&& f)
    {
        return _value_changed += f;
    }

    void remove_value_changed(size_t token)
    {
        _value_changed -= token;
    }

    ~storage() override {}

    static void register_class() noexcept
    {
        register_type<storage>("storage");
        add_constructor<storage>();
        add_property<storage, int>("value", &storage::get_value, &storage::set_value);
        add_event<storage, int>("value_changed", &storage::add_value_changed<function<void(int)>>, &storage::remove_value_changed, &storage::invoke_value_changed);
    }
};

int main()
{
    register_class<storage>();

    auto t = *get_type_index("storage");
    auto mc = construct(t);
    auto ev = get_event<int>(t, "value_changed");
    auto token = ev.add(mc, function<void(int)>([](int i) { cout << "Value changed: " << i << endl; }));
    auto prop = get_property(t, "value");
    prop.set(mc, 100);
    ev.invoke(mc, 150);
    ev.remove(mc, token);
    prop.set(mc, 200);
}
