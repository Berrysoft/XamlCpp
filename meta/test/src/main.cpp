#include <calculator.h>
#include <iostream>
#include <xaml/meta/meta_context.h>
#include <xaml/meta/type_info.h>

using namespace std;

int main()
{
    // Initialize metadata context.
    xaml_ptr<xaml_meta_context> ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&ctx));
    // A helper method to register many classes.
    XAML_THROW_IF_FAILED(xaml_test_calculator_register(ctx.get()));
    // We suppressed some failure check because we know it will success.

    // Get a type named "calculator" in namespace "test".
    xaml_ptr<xaml_type_info> t;
    XAML_THROW_IF_FAILED(ctx->get_type_by_name(to_xaml_string(U("xaml_test_calculator")).get(), (xaml_reflection_info**)&t));
    // Construct the class with default constructor.
    xaml_ptr<xaml_object> obj;
    XAML_THROW_IF_FAILED(t->construct(&obj));
    // Get the event named "value_changed".
    xaml_ptr<xaml_event_info> ev;
    XAML_THROW_IF_FAILED(t->get_event(to_xaml_string(U("value_changed")).get(), &ev));
    // Add a handler to the event of the object.
    xaml_ptr<xaml_delegate> handler;
    XAML_THROW_IF_FAILED((xaml_delegate_new<void, xaml_ptr<xaml_test_calculator>, int>(
        [](xaml_ptr<xaml_test_calculator>, int i) {
            cout << "Value changed: " << i << endl;
        },
        &handler)));
    int32_t token;
    XAML_THROW_IF_FAILED(ev->add(obj.get(), handler.get(), &token));
    {
        // Invoke the method of the object.
        // The property `value` has changed, so the event will be raised,
        // and the handler will be called.
        xaml_ptr<xaml_method_info> method;
        XAML_THROW_IF_FAILED(t->get_method(to_xaml_string(U("plus")).get(), &method));
        xaml_ptr<xaml_vector_view> args;
        XAML_THROW_IF_FAILED(xaml_delegate_pack_args(&args, obj, 1, 1));
        xaml_ptr<xaml_object> res;
        XAML_THROW_IF_FAILED(method->invoke(args.get(), &res));
    }
    // Get the property named "value".
    xaml_ptr<xaml_property_info> prop;
    XAML_THROW_IF_FAILED(t->get_property(to_xaml_string(U("value")).get(), &prop));
    // Set the int property with string.
    // It *will* success because the library converts it implicitly.
    XAML_THROW_IF_FAILED(prop->set(obj.get(), xaml_box_value(100).get()));
    XAML_THROW_IF_FAILED(prop->set(obj.get(), to_xaml_string(U("200")).get()));
    // Remove the handler.
    XAML_THROW_IF_FAILED(ev->remove(obj.get(), token));
    {
        // Although `value` has changed, the handler won't be called,
        // because the handler has been removed.
        xaml_ptr<xaml_method_info> method;
        XAML_THROW_IF_FAILED(t->get_method(to_xaml_string(U("minus")).get(), &method));
        xaml_ptr<xaml_vector_view> args;
        XAML_THROW_IF_FAILED(xaml_delegate_pack_args(&args, obj, 1, 1));
        xaml_ptr<xaml_object> res;
        XAML_THROW_IF_FAILED(method->invoke(args.get(), &res));
    }
}
