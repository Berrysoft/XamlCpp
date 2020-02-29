#ifndef XAML_UI_CONTROLS_LAYOUT_BASE_HPP
#define XAML_UI_CONTROLS_LAYOUT_BASE_HPP

#include <xaml/ui/container.hpp>

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

    class layout_base : public multicontainer
    {
    public:
        layout_base() : multicontainer() {}
        ~layout_base() override {}

#if defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
    private:
        std::unordered_map<std::shared_ptr<control>, bool> m_put_map{};
#endif // XAML_UI_GTK3

    protected:
        virtual void __draw_impl(rectangle const& region, std::function<void(std::shared_ptr<control>, rectangle const&)> func = {});

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    public:
#define ADD_LAYOUT_BASE_MEMBERS() ADD_MULTICONTAINER_MEMBERS()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_LAYOUT_BASE_HPP
