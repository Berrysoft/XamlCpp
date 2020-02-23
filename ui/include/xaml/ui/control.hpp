#ifndef XAML_UI_CONTROL_HPP
#define XAML_UI_CONTROL_HPP

#include <map>
#include <string_view>
#include <xaml/meta/conv.hpp>
#include <xaml/meta/enum_meta.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/strings.hpp>
#include <xaml/ui/drawing.hpp>
#include <xaml/ui/objc.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <optional>
#elif defined(XAML_UI_GTK3)
#include <gtk/gtk.h>
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
#import <xaml/ui/cocoa/XamlDelegate.h>
#endif

namespace xaml
{
    class container;
    class multicontainer;
    class control;

#ifdef XAML_UI_WINDOWS
    struct window_message
    {
        HWND hWnd;
        UINT Msg;
        WPARAM wParam;
        LPARAM lParam;
    };

    struct window_create_params
    {
        string_t class_name;
        string_t window_name;
        DWORD style;
        DWORD ex_style;
        int x;
        int y;
        int width;
        int height;
        control* parent;
    };
#endif

    enum class halignment_t
    {
        stretch,
        left,
        center,
        right
    };

    STRING_CONST(__halignment_stretch, "stretch")
    STRING_CONST(__halignment_left, "left")
    STRING_CONST(__halignment_center, "center")
    STRING_CONST(__halignment_right, "right")

    template <typename TChar>
    struct __halignment_enum_meta_helper
    {
        inline static __unordered_bimap<std::basic_string_view<TChar>, halignment_t> enum_map{
            { __halignment_stretch<TChar>, halignment_t::stretch },
            { __halignment_left<TChar>, halignment_t::left },
            { __halignment_center<TChar>, halignment_t::center },
            { __halignment_right<TChar>, halignment_t::right }
        };
    };

    template <typename TChar>
    struct enum_meta<halignment_t, TChar> : __enum_meta_helper<halignment_t, TChar, &__halignment_enum_meta_helper<TChar>::enum_map>
    {
    };

    enum class valignment_t
    {
        stretch,
        top,
        center,
        bottom
    };

    STRING_CONST(__valignment_stretch, "stretch")
    STRING_CONST(__valignment_top, "top")
    STRING_CONST(__valignment_center, "center")
    STRING_CONST(__valignment_bottom, "bottom")

    template <typename TChar>
    struct __valignment_enum_meta_helper
    {
    public:
        inline static __unordered_bimap<std::basic_string_view<TChar>, valignment_t> enum_map{
            { __valignment_stretch<TChar>, valignment_t::stretch },
            { __valignment_top<TChar>, valignment_t::top },
            { __valignment_center<TChar>, valignment_t::center },
            { __valignment_bottom<TChar>, valignment_t::bottom }
        };
    };

    template <typename TChar>
    struct enum_meta<valignment_t, TChar> : __enum_meta_helper<valignment_t, TChar, &__valignment_enum_meta_helper<TChar>::enum_map>
    {
    };

    class control : public meta_class_impl<control>
    {
    public:
#ifdef XAML_UI_WINDOWS
        using native_handle_type = HWND;
#elif defined(XAML_UI_GTK3)
        using native_handle_type = GtkWidget*;
#elif defined(XAML_UI_COCOA)
        using native_handle_type = OBJC_OBJECT(NSView);
#endif // XAML_UI_WINDOWS

    private:
        native_handle_type m_handle{ OBJC_NIL };

    public:
        inline native_handle_type get_handle() const noexcept { return m_handle; }
        operator bool() const noexcept { return m_handle != OBJC_NIL; }

    protected:
        void set_handle(native_handle_type h) noexcept OBJC_BLOCK({ m_handle = h; });

#ifdef XAML_UI_WINDOWS
    protected:
        XAML_UI_API void __create(window_create_params const& params);
        XAML_UI_API size __measure_text_size(string_view_t str, size offset) const;

    public:
        virtual std::optional<LRESULT> __wnd_proc(window_message const& msg) { return std::nullopt; }
#endif

#ifdef XAML_UI_COCOA
    protected:
        XAML_UI_API void __set_rect(rectangle const& real);

    public:
        using __native_delegate_type = OBJC_OBJECT(XamlDelegate);

    private:
        __native_delegate_type m_delegate{ OBJC_NIL };

    public:
        inline __native_delegate_type __get_delegate() const noexcept { return m_delegate; }

    protected:
        void __set_delegate(__native_delegate_type value) OBJC_BLOCK({ m_delegate = value; });
#endif // XAML_UI_COCOA

    public:
        XAML_UI_API control();
        XAML_UI_API virtual ~control();

        virtual void __draw(rectangle const& region) = 0;
        XAML_UI_API virtual void __size_to_fit();

        EVENT(parent_changed, control&, std::shared_ptr<control>)

    private:
        std::shared_ptr<control> m_parent{ nullptr };

    public:
        std::shared_ptr<control> get_parent() const { return m_parent; }
        XAML_UI_API void set_parent(std::shared_ptr<control> const& value);

    private:
        std::shared_ptr<meta_class> m_data_context{ nullptr };

    public:
        std::shared_ptr<meta_class> get_data_context() const { return m_data_context ? m_data_context : (m_parent ? m_parent->get_data_context() : nullptr); }
        void set_data_context(std::shared_ptr<meta_class> const& value) { m_data_context = value; }

    public:
        virtual std::shared_ptr<control> get_root_window() { return m_parent; }

    protected:
        XAML_UI_API virtual void __parent_redraw();

    private:
        EVENT(size_changed, control&, size)
        PROP_CONSTEXPR_EVENT(size, size)

    public:
        constexpr double get_width() const noexcept { return m_size.width; }
        void set_width(double value)
        {
            if (m_size.width != value)
            {
                m_size.width = value;
                m_size_changed(*this, get_size());
            }
        }

        constexpr double get_height() const noexcept { return m_size.height; }
        void set_height(double value)
        {
            if (m_size.height != value)
            {
                m_size.height = value;
                m_size_changed(*this, get_size());
            }
        }

    protected:
        void __set_size_noevent(size value) { m_size = value; }

        EVENT(margin_changed, control&, margin)
        PROP_CONSTEXPR_EVENT(margin, margin)

        EVENT(halignment_changed, control&, halignment_t)
        PROP_CONSTEXPR_EVENT(halignment, halignment_t)

        EVENT(valignment_changed, control&, valignment_t)
        PROP_CONSTEXPR_EVENT(valignment, valignment_t)

#define ADD_CONTROL_MEMBERS()   \
    ADD_PROP(data_context);     \
    ADD_PROP_EVENT(size);       \
    ADD_PROP(width);            \
    ADD_PROP(height);           \
    ADD_PROP_EVENT(margin);     \
    ADD_PROP_EVENT(halignment); \
    ADD_PROP_EVENT(valignment)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, control);
            ADD_CONTROL_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROL_HPP
