#include <calculator.h>
#include <xaml/event.h>
#include <xaml/meta/type_info.h>

struct xaml_test_calculator_impl : xaml_implement<xaml_test_calculator_impl, xaml_test_calculator, xaml_object>
{
private:
    int m_value;
    xaml_ptr<xaml_event> m_value_changed;

    xaml_result on_value_changed(int value)
    {
        xaml_ptr<xaml_object> obj;
        XAML_RETURN_IF_FAILED(box_value(value, &obj));
        xaml_ptr<xaml_vector> args;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ this, obj }, &args));
        xaml_ptr<xaml_object> res;
        return m_value_changed->invoke(args.get(), &res);
    }

public:
    xaml_test_calculator_impl()
    {
        XAML_THROW_IF_FAILED(xaml_event_new(&m_value_changed));
    }

    xaml_result XAML_CALL get_value(int* pvalue) noexcept override
    {
        *pvalue = m_value;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_value(int value) noexcept override
    {
        if (m_value != value)
        {
            m_value = value;
            return on_value_changed(value);
        }
        return XAML_S_OK;
    }
    xaml_result XAML_CALL add_value_changed(xaml_delegate* handler, std::size_t* ptoken) noexcept override
    {
        return m_value_changed->add(handler, ptoken);
    }

    xaml_result XAML_CALL remove_value_changed(std::size_t token) noexcept override
    {
        return m_value_changed->remove(token);
    }

    xaml_result XAML_CALL plus(int x, int y) noexcept override
    {
        return set_value(x + y);
    }

    xaml_result XAML_CALL minus(int x, int y) noexcept override
    {
        return set_value(x - y);
    }
};

xaml_result xaml_test_calculator_new(xaml_test_calculator** ptr) noexcept
{
    return xaml_object_new<xaml_test_calculator_impl>(ptr);
}

xaml_result xaml_test_calculator_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(info, xaml_test_calculator, "calculator.h");
    XAML_TYPE_INFO_ADD_CTOR(info, xaml_test_calculator);
    XAML_TYPE_INFO_ADD_METHOD(info, xaml_test_calculator, plus);
    XAML_TYPE_INFO_ADD_METHOD(info, xaml_test_calculator, minus);
    XAML_TYPE_INFO_ADD_PROP(info, xaml_test_calculator, value);
    XAML_TYPE_INFO_ADD_EVENT(info, xaml_test_calculator, value_changed);
    return ctx->add_type(info.get());
}
