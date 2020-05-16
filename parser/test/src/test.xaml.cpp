#include <cmath>
#include <shared/window.hpp>
#include <test.xaml.h>
#include <xaml/parser/deserializer.h>
#include <xaml/parser/parser.h>
#include <xaml/resource/resource.h>
#include <xaml/ui/application.h>
#include <xaml/ui/msgbox.h>

namespace colors
{
#include <xaml/ui/colors.h>
}

using namespace std;

struct xaml_test_window_internal : xaml_window_internal
{
    xaml_ptr<xaml_meta_context> m_ctx;

    xaml_result XAML_CALL init() noexcept;

    XAML_PROP_PTR_IMPL(model, xaml_test_model)

    xaml_result XAML_CALL on_button_click(xaml_button*) noexcept;
    xaml_result XAML_CALL on_canvas_redraw(xaml_canvas*, xaml_drawing_context*) noexcept;
};

struct xaml_test_window_impl : xaml_window_implement<xaml_test_window_impl, xaml_test_window_internal, xaml_test_window>
{
    xaml_result XAML_CALL init(xaml_meta_context* ctx) noexcept
    {
        m_internal.m_ctx = ctx;
        return m_internal.init();
    }

    XAML_PROP_PTR_INTERNAL_IMPL(model, xaml_test_model)

    xaml_result XAML_CALL on_button_click(xaml_button* btn) noexcept override { return m_internal.on_button_click(btn); }
    xaml_result XAML_CALL on_canvas_redraw(xaml_canvas* cv, xaml_drawing_context* dc) noexcept override { return m_internal.on_canvas_redraw(cv, dc); }
};

xaml_result xaml_test_window_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_window_internal::init());
    xaml_ptr<xaml_string> path;
    XAML_RETURN_IF_FAILED(xaml_string_new_view(U("view/test.xaml"), &path));
    xaml_ptr<xaml_buffer> data;
    XAML_RETURN_IF_FAILED(xaml_resource_get(path.get(), &data));
    xaml_ptr<xaml_node> node;
    xaml_ptr<xaml_vector_view> headers;
    XAML_RETURN_IF_FAILED(xaml_parser_parse_buffer(m_ctx.get(), data.get(), &node, &headers));
    XAML_RETURN_IF_FAILED(xaml_parser_deserialize_inplace(m_ctx.get(), node.get(), m_outer_this));
    return XAML_S_OK;
}

xaml_result xaml_test_window_internal::on_button_click(xaml_button* btn) noexcept
{
    xaml_ptr<xaml_string> text;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &text));
    XAML_RETURN_IF_FAILED(m_model->set_text(text.get()));
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello"), &title));
    xaml_ptr<xaml_string> des;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("I'm going to say hello..."), &des));
    xaml_msgbox_result res;
    XAML_RETURN_IF_FAILED(xaml_msgbox(static_cast<xaml_window*>(m_outer_this), text.get(), title.get(), des.get(), xaml_msgbox_info, xaml_msgbox_buttons_abort_retry_ignore, &res));
    return XAML_S_OK;
}

xaml_result xaml_test_window_internal::on_canvas_redraw(xaml_canvas* cv, xaml_drawing_context* dc) noexcept
{
    xaml_ptr<xaml_application> current_app;
    XAML_RETURN_IF_FAILED(xaml_application_current(&current_app));
    xaml_application_theme theme;
    XAML_RETURN_IF_FAILED(current_app->get_theme(&theme));
    bool is_dark = theme == xaml_application_theme_dark;
    xaml_size csize;
    XAML_RETURN_IF_FAILED(cv->get_size(&csize));
    auto cx = csize.width / 2;
    auto cy = csize.height / 2;
    auto r = (min)(cx, cy) - 2;
    XAML_RETURN_IF_FAILED(dc->draw_ellipse({ is_dark ? colors::white : colors::black, 1 }, { cx - r, cy - r, r * 2, r * 2 }));
    xaml_point pc{ cx, cy };
    xaml_point p1 = xaml_point{ 0, -r } + pc;
    xaml_point p2 = xaml_point{ r * sqrt(3) / 2, r / 2 } + pc;
    xaml_point p3 = xaml_point{ -r * sqrt(3) / 2, r / 2 } + pc;
    XAML_RETURN_IF_FAILED(dc->draw_line({ colors::sky_blue, 1 }, p1, p2));
    XAML_RETURN_IF_FAILED(dc->draw_line({ colors::sky_blue, 1 }, p2, p3));
    XAML_RETURN_IF_FAILED(dc->draw_line({ colors::sky_blue, 1 }, p3, p1));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_test_window_new(xaml_meta_context* ctx, xaml_test_window** ptr) noexcept
{
    return xaml_object_init<xaml_test_window_impl>(ptr, ctx);
}

xaml_result XAML_CALL xaml_test_window_register(xaml_meta_context* ctx) noexcept
{
    xaml_ptr<xaml_string> xml_ns;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("https://github.com/Berrysoft/XamlCpp/parser/test/"), &xml_ns));
    xaml_ptr<xaml_string> ns;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_test"), &ns));
    XAML_RETURN_IF_FAILED(ctx->add_namespace(xml_ns.get(), ns.get()));

    XAML_TYPE_INFO_NEW(xaml_test_window, "test.xaml.h");
    XAML_RETURN_IF_FAILED(xaml_window_members(__info.get()));
    XAML_TYPE_INFO_ADD_PROP(model, xaml_test_model);
    XAML_TYPE_INFO_ADD_METHOD(on_button_click);
    XAML_TYPE_INFO_ADD_METHOD(on_canvas_redraw);
    return ctx->add_type(__info.get());
}
