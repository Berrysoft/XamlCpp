#include <atomic>
#include <map>
#include <xaml/event.h>
#include <xaml/object.h>
#include <xaml/ptr.hpp>

using namespace std;

struct xaml_event_impl : xaml_implement<xaml_event_impl, xaml_event, xaml_delegate, xaml_object>
{
private:
    atomic<int32_t> index{ 0 };
    map<int32_t, xaml_ptr<xaml_delegate>> m_callbacks{};

public:
    xaml_result XAML_CALL add(xaml_delegate* handler, int32_t* ptoken) noexcept override
    {
        int32_t token = ++index;
        try
        {
            m_callbacks.emplace(token, handler);
            *ptoken = token;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }

    xaml_result XAML_CALL remove(int32_t token) noexcept override
    {
        auto it = m_callbacks.find(token);
        if (it == m_callbacks.end()) return XAML_E_INVALIDARG;
        m_callbacks.erase(it);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL invoke(xaml_vector_view* args, xaml_object** ptr) noexcept override
    {
        for (auto& pair : m_callbacks)
        {
            xaml_ptr<xaml_object> obj;
            xaml_result hr = pair.second->invoke(args, &obj);
            if (XAML_FAILED(hr)) return hr;
        }
        *ptr = nullptr;
        return XAML_S_OK;
    }
};

xaml_result xaml_event_new(xaml_event** ptr) noexcept
{
    return xaml_object_new<xaml_event_impl>(ptr);
}
