#include <iostream>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;

class storage : public meta_class_impl<storage>
{
private:
    int _value;

public:
    int get_value() const noexcept { return _value; }
    void set_value(int v) noexcept { _value = v; }

    static void register_class() noexcept
    {
        register_type<storage>("storage");
        add_constructor<storage>();
        add_property<storage, int>("value", &storage::get_value, &storage::set_value);
    }
};

int main()
{
    register_class<storage>();

    auto t = *get_type_index("storage");
    auto mc = construct(t);
    auto prop = get_property(t, "value");
    prop.set(mc, 100);
    cout << any_cast<int>(prop.get(mc)) << endl;
}
