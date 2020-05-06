#include <shared/atomic_guard.hpp>
#include <shared/webview.hpp>
#include <win/webview_ie.hpp>
#include <xaml/ui/controls/webview.h>
#include <xaml/ui/win/dpi.h>

#ifdef XAML_UI_WEBVIEW_EDGE
#include <win/webview_edge.hpp>
#endif // XAML_UI_WEBVIEW_EDGE

#ifdef XAML_UI_WEBVIEW_WEBVIEW2
#include <win/webview_edge2.hpp>
#endif // XAML_UI_WEBVIEW_WEBVIEW2

using namespace std;

xaml_result xaml_webview_internal::create_edge2(xaml_rectangle const& real) noexcept
{
#ifdef XAML_UI_WEBVIEW_WEBVIEW2
    m_webview = make_unique<xaml_webview_edge2>();
    return m_webview->create_async(m_handle, real, [this, real]() -> xaml_result {
        if (!*m_webview)
        {
            return create_edge(real);
        }
        else
        {
            return draw_create();
        }
    });
#else
    return create_edge(real);
#endif // XAML_UI_WEBVIEW_WEBVIEW2
}

xaml_result xaml_webview_internal::create_edge(xaml_rectangle const& real) noexcept
{
#ifdef XAML_UI_WEBVIEW_EDGE
    m_webview = make_unique<xaml_webview_edge>();
    return m_webview->create_async(m_handle, real, [this, real]() -> xaml_result {
        if (!*m_webview)
        {
            return create_ie(real);
        }
        else
        {
            return draw_create();
        }
    });
#else
    return create_ie(real);
#endif // XAML_UI_WEBVIEW_EDGE
}

xaml_result xaml_webview_internal::create_ie(xaml_rectangle const& real) noexcept
{
    m_webview = make_unique<xaml_webview_ie>();
    return m_webview->create_async(m_handle, real, [this]() -> xaml_result {
        if (!*m_webview)
        {
            m_webview = nullptr;
            return XAML_S_OK;
        }
        else
        {
            return draw_create();
        }
    });
}

xaml_result xaml_webview_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        xaml_ptr<xaml_win32_control> native_parent;
        XAML_RETURN_IF_FAILED(m_parent->query(&native_parent));
        XAML_RETURN_IF_FAILED(native_parent->get_handle(&m_handle));
        xaml_rectangle real = region - m_margin;
        UINT udpi = XamlGetDpiForWindow(m_handle);
        xaml_rectangle real_real = real * udpi / USER_DEFAULT_SCREEN_DPI;
        if (!m_webview)
        {
            XAML_RETURN_IF_FAILED(create_edge2(real_real));
        }
        XAML_RETURN_IF_FAILED(set_size_noevent({ real.width, real.height }));
        if (m_webview) m_webview->set_rect(real_real);
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::draw_create() noexcept
{
    m_webview->set_navigated([this](xaml_char_t const* uri) -> xaml_result {
        xaml_atomic_guard guard(m_navigating);
        if (!guard.test_and_set())
        {
            xaml_ptr<xaml_string> uri_str;
            XAML_RETURN_IF_FAILED(xaml_string_new(uri, &uri_str));
            return set_uri(uri_str.get());
        }
        return XAML_S_OK;
    });
    m_webview->set_resource_requested([this](xaml_ptr<xaml_webview_resource_requested_args> args) -> xaml_result {
        return on_resource_requested(m_outer_this, args);
    });
    m_created.store(true);
    XAML_RETURN_IF_FAILED(draw_visible());
    XAML_RETURN_IF_FAILED(draw_uri());
    XAML_RETURN_IF_FAILED(parent_redraw());
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::draw_size() noexcept
{
    if (m_created)
    {
        xaml_size size;
        XAML_RETURN_IF_FAILED(get_real_size(&size));
        return m_webview->set_size(size);
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::draw_visible() noexcept
{
    if (m_created)
    {
        return m_webview->set_visible(m_is_visible);
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::draw_uri() noexcept
{
    if (m_created)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(m_uri->get_data(&data));
        return m_webview->navigate(data);
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::go_forward() noexcept
{
    if (m_created)
    {
        return m_webview->go_forward();
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::go_back() noexcept
{
    if (m_created)
    {
        return m_webview->go_back();
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::get_can_go_forward(bool* pvalue) noexcept
{
    if (m_created)
    {
        return m_webview->get_can_go_forward(pvalue);
    }
    else
    {
        *pvalue = false;
        return XAML_S_OK;
    }
}

xaml_result xaml_webview_internal::get_can_go_back(bool* pvalue) noexcept
{
    if (m_created)
    {
        return m_webview->get_can_go_back(pvalue);
    }
    else
    {
        *pvalue = false;
        return XAML_S_OK;
    }
}
