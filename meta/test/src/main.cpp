#include <calculator.h>
#include <nowide/iostream.hpp>
#include <sf/format.hpp>
#include <xaml/meta/meta_context.h>
#include <xaml/meta/type_info.h>

using namespace std;

int main()
{
    // Initialize metadata context.
    xaml_ptr<xaml_meta_context> ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&ctx));
    // A helper method to register many classes.
    XAML_THROW_IF_FAILED(xaml_test_calculator_register(ctx));
    // We suppressed some failure check because we know it will success.

    // Get a type named "calculator" in namespace "test".
    xaml_ptr<xaml_type_info> t;
    XAML_THROW_IF_FAILED(ctx->get_type_by_name(xaml_box_value(U("xaml_test_calculator")), (xaml_reflection_info**)&t));
    // Construct the class with default constructor.
    xaml_ptr<xaml_object> obj;
    XAML_THROW_IF_FAILED(t->construct(&obj));
    // Get the event named "value_changed".
    xaml_ptr<xaml_event_info> ev;
    XAML_THROW_IF_FAILED(t->get_event(xaml_box_value(U("value_changed")), &ev));
    // Add a handler to the event of the object.
    xaml_ptr<xaml_method_info> handler;
    XAML_THROW_IF_FAILED((xaml_method_info_new<xaml_test_calculator, int>(
        nullptr,
        [](xaml_test_calculator*, int i) -> xaml_result {
            sf::println(nowide::cout, "Value changed: {}", i);
            return XAML_S_OK;
        },
        &handler)));
    int32_t token;
    XAML_THROW_IF_FAILED(ev->add(obj, handler, &token));
    {
        // Invoke the method of the object.
        // The property `value` has changed, so the event will be raised,
        // and the handler will be called.
        xaml_ptr<xaml_method_info> method;
        XAML_THROW_IF_FAILED(t->get_method(xaml_box_value(U("plus")), &method));
        xaml_ptr<xaml_vector_view<xaml_object>> args;
        XAML_THROW_IF_FAILED(xaml_delegate_pack_args(&args, obj, 1, 1));
        xaml_ptr<xaml_object> res;
        XAML_THROW_IF_FAILED(method->invoke(args, &res));
    }
    // Get the property named "value".
    xaml_ptr<xaml_property_info> prop;
    XAML_THROW_IF_FAILED(t->get_property(xaml_box_value(U("value")), &prop));
    // Set the int property with string.
    // It *will* success because the library converts it implicitly.
    XAML_THROW_IF_FAILED(prop->set(obj, xaml_box_value(100)));
    XAML_THROW_IF_FAILED(prop->set(obj, xaml_box_value(U("200"))));
    // Remove the handler.
    XAML_THROW_IF_FAILED(ev->remove(obj, token));
    {
        // Although `value` has changed, the handler won't be called,
        // because the handler has been removed.
        xaml_ptr<xaml_method_info> method;
        XAML_THROW_IF_FAILED(t->get_method(xaml_box_value(U("minus")), &method));
        xaml_ptr<xaml_vector_view<xaml_object>> args;
        XAML_THROW_IF_FAILED(xaml_delegate_pack_args(&args, obj, 1, 1));
        xaml_ptr<xaml_object> res;
        XAML_THROW_IF_FAILED(method->invoke(args, &res));
    }
}
