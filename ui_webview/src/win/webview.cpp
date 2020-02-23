#include <shared/atomic_guard.hpp>
#include <win/webview_edge.hpp>
#include <win/webview_edge2.hpp>
#include <win/webview_ie.hpp>
#include <xaml/ui/webview/webview.hpp>

using namespace std;

namespace xaml
{
    void webview::__draw(rectangle const& region)
    {
        set_handle(get_parent()->get_handle());
        rectangle real = region - get_margin();
        if (!__get_webview())
        {
            m_webview = make_shared<webview_edge>();
            m_webview->create_async(get_handle(), real, [this, real]() {
                if (!*m_webview)
                {
                    m_webview = make_shared<webview_edge>();
                    m_webview->create_async(get_handle(), real, [this, real]() {
                        if (!*m_webview)
                        {
                            m_webview = make_shared<webview_ie>();
                            m_webview->create_async(get_handle(), real, [this]() {
                                if (!*m_webview)
                                {
                                    m_webview = nullptr;
                                }
                                else
                                {
                                    draw_create();
                                }
                            });
                        }
                        else
                        {
                            draw_create();
                        }
                    });
                }
                else
                {
                    draw_create();
                }
            });
        }
        __set_size_noevent({ real.width, real.height });
        if (__get_webview() && *__get_webview()) m_webview->set_rect(real);
    }

    void webview::draw_create()
    {
        m_webview->set_navigated([this](string_view_t uri) {
            atomic_guard guard(m_navigating);
            if (!guard.exchange(true))
            {
                set_uri(uri);
            }
        });
        m_webview->set_resource_requested([this](resource_requested_args& args) { m_resource_requested(*this, args); });
        draw_uri();
        __parent_redraw();
    }

    void webview::draw_size()
    {
        if (__get_webview() && *__get_webview())
        {
            m_webview->set_size(get_size());
        }
    }

    void webview::draw_uri()
    {
        if (__get_webview() && *__get_webview())
        {
            m_webview->navigate(get_uri());
        }
    }
} // namespace xaml
