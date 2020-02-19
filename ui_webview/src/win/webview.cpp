#include <xaml/ui/webview/webview.hpp>

#include <win/webview_edge.hpp>
#include <win/webview_edge2.hpp>
#include <win/webview_ie.hpp>

using namespace std;

namespace xaml
{
    void webview::__draw(rectangle const& region)
    {
        set_handle(get_parent()->get_handle());
        rectangle real = region - get_margin();
        if (!__get_webview())
        {
            m_webview = make_shared<webview_edge2>();
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
                                    draw_uri();
                                    __parent_redraw();
                                }
                            });
                        }
                        else
                        {
                            draw_uri();
                            __parent_redraw();
                        }
                    });
                }
                else
                {
                    draw_uri();
                    __parent_redraw();
                }
            });
        }
        __set_size_noevent({ real.width, real.height });
        if (__get_webview() && *__get_webview()) m_webview->set_rect(real);
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
