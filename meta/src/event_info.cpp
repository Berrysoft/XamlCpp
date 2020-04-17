#include <xaml/meta/event_info.h>
#include <xaml/xaml_ptr.hpp>

using namespace std;

struct xaml_event_info_impl : xaml_implement<xaml_event_info_impl, xaml_event_info, xaml_object>
{
private:
    xaml_ptr<xaml_string> m_name;
    function<xaml_result XAML_CALL(xaml_object*, xaml_delegate*, size_t*)> m_adder;
    function<xaml_result XAML_CALL(xaml_object*, size_t)> m_remover;

public:
    xaml_event_info_impl(xaml_ptr<xaml_string>&& name, function<xaml_result XAML_CALL(xaml_object*, xaml_delegate*, size_t*)>&& adder, function<xaml_result XAML_CALL(xaml_object*, size_t)>&& remover)
        : m_name(move(name)), m_adder(move(adder)), m_remover(move(remover)) {}

    xaml_result XAML_CALL get_name(xaml_string** ptr) noexcept override
    {
        m_name->add_ref();
        *ptr = m_name.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add(xaml_object* target, xaml_delegate* handler, size_t* ptoken) noexcept override
    {
        if (m_adder)
        {
            return m_adder(target, handler, ptoken);
        }
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL remove(xaml_object* target, size_t token) noexcept override
    {
        if (m_remover)
        {
            return m_remover(target, token);
        }
        return XAML_E_NOTIMPL;
    }
};

xaml_result xaml_event_info_new(xaml_string* name, xaml_result(XAML_CALL* adder)(xaml_object*, xaml_delegate*, size_t*), xaml_result(XAML_CALL* remover)(xaml_object*, size_t), xaml_event_info** ptr) noexcept
{
    return xaml_object_new<xaml_event_info_impl>(ptr, name, adder, remover);
}

xaml_result xaml_event_info_new(xaml_string* name, function<xaml_result XAML_CALL(xaml_object*, xaml_delegate*, size_t*)>&& adder, function<xaml_result XAML_CALL(xaml_object*, size_t)>&& remover, xaml_event_info** ptr) noexcept
{
    return xaml_object_new<xaml_event_info_impl>(ptr, name, move(adder), move(remover));
}
