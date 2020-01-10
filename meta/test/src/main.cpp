#include <iostream>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/meta_macro.hpp>

using namespace std;
using namespace xaml;

// The class should inherit `xaml::meta_class`.
// `xaml_class_impl` is a helper class to implement some key features of `meta_class`.
class calculator : public meta_class_impl<calculator>
{
public:
    // Adds a property named "value" with type "int",
    // and raise "value_changed" event when it changes.
    // To simply get or set its value, call `int get_value()`
    // or `void set_value(int)`.
    PROP_EVENT(value, int)

    // Adds an event named "value_changed" with signature
    // `void(calculator const&, int)`.
    // It is necessary for the property above.
    // To simply add a handler, call `size_t add_value_changed(function<void(calculator const&,int)>)`;
    // to remove it, call `void remove_value_changed(size_t)`,
    // the parameter is the return value of the add method.
    EVENT(value_changed, calculator const&, int)

public:
    void plus(int x, int y) { set_value(x + y); }
    void minus(int x, int y) { set_value(x - y); }

    static int multiply(int x, int y) { return x * y; }

    ~calculator() override {}

    static void register_class() noexcept
    {
        // Register a type named "calculator" in namespace "test".
        // The namespace is optional.
        REGISTER_TYPE(test, calculator);
        // Add default constructor.
        ADD_CTOR_DEF();
        // Add methods `plus` and `minus`.
        ADD_METHOD(plus);
        ADD_METHOD(minus);
        // Add static method `multiply`.
        ADD_STATIC_METHOD(multiply);
        // Add property `value` and event `value_changed`.
        ADD_PROP_EVENT(value);
    }
};

int main()
{
    // A helper method to register many classes.
    register_class<calculator>();

    // We suppressed some failure check because we know it will success.

    // Get a type named "calculator" in namespace "test".
    auto t = *get_type("test", "calculator");
    // Construct the class with default constructor.
    auto mc = construct(t);
    // Get the event named "value_changed".
    auto ev = get_event<calculator const&, int>(t, "value_changed");
    // Add a handler to the event of the object.
    auto token = ev.add(mc, function<void(calculator const&, int)>([](calculator const&, int i) { cout << "Value changed: " << i << endl; }));
    // Invoke the method of the object.
    // The property `value` has changed, so the event will be raised,
    // and the handler will be called.
    invoke_method<void>(mc, "plus", 1, 1);
    // Get the property named "value".
    auto prop = get_property(t, "value");
    // Set the int property with string.
    // It *will* success because the library converts it implicitly.
    prop.set(mc, "100");
    // Remove the handler.
    ev.remove(mc, token);
    // Although `value` has changed, the handler won't be called,
    // because the handler has been removed.
    invoke_method<void>(mc, "minus", 1, 1);
    // Invoke the static method.
    cout << "3 * 7 = " << *invoke_static_method<int>(t, "multiply", 3, 7) << endl;
}
