#include <iostream>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/observable_vector.hpp>

using namespace std;
using namespace xaml;

// The class should inherit `xaml::meta_class`.
// `xaml_class_impl` is a helper class to implement some key features of `meta_class`.
class calculator : public meta_class
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
    // Initialize metadata context.
    init_context();
    // A helper method to register many classes.
    register_class<calculator>();

    // We suppressed some failure check because we know it will success.

    // Get a type named "calculator" in namespace "test".
    auto t = *get_type("test", "calculator");
    // Construct the class with default constructor.
    // It is as same as calling operator new, so you should manully wrap it.
    unique_ptr<meta_class> mc{ construct(t) };
    // Get the event named "value_changed".
    auto ev = get_event<calculator const&, int>(t, "value_changed");
    // Add a handler to the event of the object.
    auto token = ev.add(mc.get(), function<void(calculator const&, int)>([](calculator const&, int i) { cout << "Value changed: " << i << endl; }));
    // Invoke the method of the object.
    // The property `value` has changed, so the event will be raised,
    // and the handler will be called.
    invoke_method<void>(mc.get(), "plus", 1, 1);
    // Get the property named "value".
    auto prop = get_property(t, "value");
    // Set the int property with string.
    // It *will* success because the library converts it implicitly.
    prop.set(mc.get(), "100");
    prop.set(mc.get(), L"200"sv);
    // Remove the handler.
    ev.remove(mc.get(), token);
    // Although `value` has changed, the handler won't be called,
    // because the handler has been removed.
    invoke_method<void>(mc.get(), "minus", 1, 1);
    // Invoke the static method.
    cout << "3 * 7 = " << *invoke_static_method<int>(t, "multiply", 3, 7) << endl;
    cout << endl;
    observable_vector<int> obs{ 1, 2, 3 };
    obs.add_vector_changed([](observable_vector<int>&, vector_changed_args<int>& args) {
        switch (args.action)
        {
        case vector_changed_action::add:
            cout << "Add items at " << args.new_index << ": ";
            for (auto& item : args.new_items)
            {
                cout << item << ' ';
            }
            cout << endl;
            break;
        case vector_changed_action::erase:
            cout << "Erase items at " << args.old_index << ": ";
            for (auto& item : args.old_items)
            {
                cout << item << ' ';
            }
            cout << endl;
            break;
        case vector_changed_action::move:
            cout << "Move item " << args.old_items[0] << " at " << args.old_index << " to " << args.new_index << '.' << endl;
            break;
        case vector_changed_action::replace:
            cout << "Replace item at " << args.old_index << " from " << args.old_items[0] << " to " << args.new_items[0] << '.' << endl;
            break;
        case vector_changed_action::reset:
            cout << "Reset. Old count: " << args.old_items.size() << "; new count: " << args.new_items.size() << '.' << endl;
            break;
        }
    });
    obs.push_back(4);
    obs.pop_back();
    obs.insert(obs.begin() + 2, 5);
    for (auto item : obs)
    {
        item = item + 1;
    }
    obs = { 4, 5, 6, 7, 8 };
    obs.clear();
}
