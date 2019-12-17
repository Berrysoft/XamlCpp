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
    static constexpr std::string_view class_name = "storage";

    static constexpr std::string_view value_prop_name = "value";
    int get_value() const noexcept { return _value; }
    void set_value(int v) noexcept
    {
        if (_value != v)
        {
            _value = v;
            invoke_value_changed(v);
        }
    }

    static constexpr std::string_view value_changed_event_name = "value_changed";
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
        register_type<storage>(class_name);
        add_constructor<storage>();
        add_property<storage, int>(value_prop_name, &storage::get_value, &storage::set_value);
        add_event<storage, int>(value_changed_event_name, &storage::add_value_changed<function<void(int)>>, &storage::remove_value_changed, &storage::invoke_value_changed);
    }
};

int main()
{
    register_class<storage>();

    auto t = *get_type_index(storage::class_name);
    auto mc = construct(t);
    auto ev = get_event<int>(t, storage::value_changed_event_name);
    auto token = ev.add(mc, function<void(int)>([](int i) { cout << "Value changed: " << i << endl; }));
    auto prop = get_property(t, storage::value_prop_name);
    prop.set(mc, 100);
    ev.invoke(mc, 150);
    ev.remove(mc, token);
    prop.set(mc, 200);
}
