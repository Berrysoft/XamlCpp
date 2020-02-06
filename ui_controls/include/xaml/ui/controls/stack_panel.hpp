#ifndef XAML_UI_CONTROLS_STACK_PANEL_HPP
#define XAML_UI_CONTROLS_STACK_PANEL_HPP

#include <xaml/ui/controls/layout_base.hpp>

namespace xaml
{
    enum class orientation
    {
        horizontal,
        vertical
    };

    STRING_CONST(__orientation_horizontal, "horizontal")
    STRING_CONST(__orientation_vertical, "vertical")

    template <typename TChar>
    struct __orientation_enum_meta_helper
    {
        inline static __unordered_bimap<std::basic_string_view<TChar>, orientation> enum_map{
            { __orientation_horizontal<TChar>, orientation::horizontal },
            { __orientation_vertical<TChar>, orientation::vertical }
        };
    };

    template <typename TChar>
    struct enum_meta<orientation, TChar> : __enum_meta_helper<orientation, TChar, &__orientation_enum_meta_helper<TChar>::enum_map>
    {
    };

    class stack_panel : public layout_base
    {
    public:
        XAML_UI_CONTROLS_API stack_panel();
        XAML_UI_CONTROLS_API ~stack_panel() override;

    private:
        XAML_UI_CONTROLS_API void __draw_impl(rectangle const& region, std::function<void(std::shared_ptr<control>, rectangle const&)> func = {}) override;

    public:
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    public:
        PROP_CONSTEXPR(columns, std::size_t)
        PROP_CONSTEXPR(rows, std::size_t)
        PROP_CONSTEXPR(orientation, orientation)

    public:
#define ADD_STACK_PANEL_MEMBERS() \
    ADD_MULTICONTAINER_MEMBERS(); \
    ADD_PROP(columns);            \
    ADD_PROP(rows);               \
    ADD_PROP(orientation)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, stack_panel);
            ADD_CTOR_DEF();
            ADD_STACK_PANEL_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_STACK_PANEL_HPP
