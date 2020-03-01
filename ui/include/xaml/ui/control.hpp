#ifndef XAML_UI_CONTROL_HPP
#define XAML_UI_CONTROL_HPP

#include <map>
#include <memory>
#include <optional>
#include <string_view>
#include <xaml/meta/conv.hpp>
#include <xaml/meta/enum_meta.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/strings.hpp>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    class container;
    class multicontainer;
    class control;

#ifdef XAML_UI_WINDOWS
    struct window_message;
    struct window_create_params;
#endif // XAML_UI_WINDOWS

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

    struct native_control;

    class control : public meta_class
    {
    public:
        using native_handle_type = std::shared_ptr<native_control>;

    private:
        native_handle_type m_handle{ nullptr };

    public:
        inline native_handle_type get_handle() const noexcept { return m_handle; }
        operator bool() const noexcept { return (bool)m_handle; }

    protected:
        void set_handle(native_handle_type h) noexcept { m_handle = h; }

#ifdef XAML_UI_WINDOWS
    protected:
        XAML_UI_API void __create(window_create_params const& params);
        XAML_UI_API size __measure_text_size(string_view_t str, size offset) const;

    public:
        virtual std::optional<std::intptr_t> __wnd_proc(window_message const& msg) { return std::nullopt; }
#endif

    protected:
        XAML_UI_API void __set_rect(rectangle const& region);

    public:
        XAML_UI_API control();
        XAML_UI_API virtual ~control();

        virtual void __draw(rectangle const& region) = 0;
        XAML_UI_API virtual void __size_to_fit();

        EVENT(parent_changed, control&, control&)

    private:
        std::weak_ptr<control> m_parent{};

    public:
        std::weak_ptr<control> get_parent() const { return m_parent; }
        XAML_UI_API void set_parent(std::weak_ptr<control> value);

    private:
        std::shared_ptr<meta_class> m_data_context{ nullptr };

    public:
        std::shared_ptr<meta_class> get_data_context() const { return m_data_context ? m_data_context : (m_parent.expired() ? nullptr : m_parent.lock()->get_data_context()); }
        void set_data_context(std::shared_ptr<meta_class> const& value) { m_data_context = value; }

    public:
        virtual std::shared_ptr<control> get_root_window() { return m_parent.expired() ? nullptr : m_parent.lock()->get_root_window(); }

    protected:
        XAML_UI_API virtual void __parent_redraw();

    private:
        EVENT(size_changed, control&, size)
        PROP_CONSTEXPR_EVENT(size, size)

    public:
        XAML_UI_API size __get_real_size() const;
        XAML_UI_API void __set_real_size(size value);

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

    public:
        XAML_UI_API margin __get_real_margin() const;
        XAML_UI_API void __set_real_margin(margin const& value);

        EVENT(halignment_changed, control&, halignment_t)
        PROP_CONSTEXPR_EVENT(halignment, halignment_t)

        EVENT(valignment_changed, control&, valignment_t)
        PROP_CONSTEXPR_EVENT(valignment, valignment_t)

    protected:
        XAML_UI_API virtual void draw_size();

    public:
#define ADD_CONTROL_MEMBERS()   \
    ADD_PROP(data_context);     \
    ADD_PROP_EVENT(size);       \
    ADD_PROP(width);            \
    ADD_PROP(height);           \
    ADD_PROP_EVENT(margin);     \
    ADD_PROP_EVENT(halignment); \
    ADD_PROP_EVENT(valignment)

        REGISTER_CLASS_DECL(xaml, control)
        {
            ADD_CONTROL_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROL_HPP
