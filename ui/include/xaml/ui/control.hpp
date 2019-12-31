#ifndef XAML_UI_CONTROL_HPP
#define XAML_UI_CONTROL_HPP

#include <map>
#include <string_view>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/drawing.hpp>
#include <xaml/ui/objc.hpp>
#include <xaml/ui/strings.hpp>

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
    struct enum_meta<halignment_t, TChar>
    {
    private:
        inline static std::map<std::basic_string_view<TChar>, halignment_t> enum_map{
            { __halignment_stretch<TChar>, halignment_t::stretch },
            { __halignment_left<TChar>, halignment_t::left },
            { __halignment_center<TChar>, halignment_t::center },
            { __halignment_right<TChar>, halignment_t::right }
        };

    public:
        inline halignment_t operator()(std::basic_string_view<TChar> str) const noexcept
        {
            return enum_map[str];
        }
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
    struct enum_meta<valignment_t, TChar>
    {
    private:
        inline static std::map<std::basic_string_view<TChar>, valignment_t> enum_map{
            { __valignment_stretch<TChar>, valignment_t::stretch },
            { __valignment_top<TChar>, valignment_t::top },
            { __valignment_center<TChar>, valignment_t::center },
            { __valignment_bottom<TChar>, valignment_t::bottom }
        };

    public:
        inline valignment_t operator()(std::basic_string_view<TChar> str) const noexcept
        {
            return enum_map[str];
        }
    };

    class control : public std::enable_shared_from_this<control>
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
        native_handle_type m_handle{ nullptr };

    public:
        inline native_handle_type get_handle() const noexcept { return m_handle; }
        operator bool() const noexcept { return m_handle; }

    protected:
        void set_handle(native_handle_type h) noexcept OBJC_BLOCK({ m_handle = h; });

#ifdef XAML_UI_WINDOWS
    protected:
        void __create(window_create_params const& params);
        size __measure_text_size(string_view_t str) const;

    public:
        virtual std::optional<LRESULT> __wnd_proc(window_message const& msg) { return std::nullopt; }
#endif

#ifdef XAML_UI_COCOA
    public:
        using __native_delegate_type = OBJC_OBJECT(XamlDelegate);

    private:
        __native_delegate_type m_delegate;

    public:
        inline __native_delegate_type __get_delegate() const noexcept { return m_delegate; }

    protected:
        void __set_delegate(__native_delegate_type value) OBJC_BLOCK({ m_delegate = value; });
#endif // XAML_UI_COCOA

    public:
        control();
        virtual ~control();

        virtual void __draw(rectangle const& region) = 0;
        virtual void __size_to_fit();

        EVENT(parent_changed, control const&, std::shared_ptr<control>)

    private:
        std::shared_ptr<control> m_parent{ nullptr };

    public:
        std::shared_ptr<control> get_parent() const { return m_parent; }
        void set_parent(std::shared_ptr<control> const& value);

    private:
        std::shared_ptr<meta_class> m_data_context{ nullptr };

    public:
        std::shared_ptr<meta_class> get_data_context() const { return m_data_context ? m_data_context : (m_parent ? m_parent->get_data_context() : nullptr); }
        void set_data_context(std::shared_ptr<meta_class> const& value) { m_data_context = value; }

    public:
        virtual std::shared_ptr<control> get_root_window() { return m_parent; }

    public:
        virtual bool is_container() const = 0;
        virtual bool is_multicontainer() const = 0;

    protected:
        virtual void __parent_redraw();

    private:
        size m_size{ 0, 0 };

        EVENT(size_changed, control const&, size)

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

        constexpr size get_size() const { return m_size; }
        void set_size(size value)
        {
            if (m_size != value)
            {
                m_size = value;
                m_size_changed(*this, get_size());
            }
        }

    protected:
        void __set_size_noevent(size value) { m_size = value; }

        EVENT(margin_changed, control const&, margin)

    private:
        margin m_margin{ 0, 0, 0, 0 };

    public:
        constexpr margin get_margin() const noexcept { return m_margin; }
        void set_margin(margin const& value)
        {
            if (m_margin != value)
            {
                m_margin = value;
                m_margin_changed(*this, value);
            }
        }

        PROP(halignment, halignment_t)
        PROP(valignment, valignment_t)

#define ADD_CONTROL_MEMBERS() \
    ADD_PROP(data_context);   \
    ADD_PROP_EVENT(size);     \
    ADD_PROP(width);          \
    ADD_PROP(height);         \
    ADD_PROP_EVENT(margin);   \
    ADD_PROP(halignment);     \
    ADD_PROP(valignment);     \
    ADD_METHOD(is_container); \
    ADD_METHOD(is_multicontainer)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, control);
            ADD_CONTROL_MEMBERS();
        }
    };

    class common_control : public control
    {
    public:
        common_control() : control() {}
        virtual ~common_control() override {}

        bool is_container() const override final { return false; }
        bool is_multicontainer() const override final { return false; }

#define ADD_COMMON_CONTROL_MEMBERS() ADD_CONTROL_MEMBERS()

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, common_control);
            ADD_COMMON_CONTROL_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROL_HPP
