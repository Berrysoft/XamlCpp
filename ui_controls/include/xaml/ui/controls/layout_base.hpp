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
    struct enum_meta<orientation, TChar> : __enum_meta_helper<enum_meta<orientation, TChar>>
    {
        typename __enum_meta_helper<enum_meta<orientation, TChar>>::map_type enum_map{
            { __orientation_horizontal<TChar>, orientation::horizontal },
            { __orientation_vertical<TChar>, orientation::vertical }
        };
    };

    template <>
    struct type_guid<meta_box<orientation>>
    {
        static constexpr guid value{ 0xbf0f301a, 0xeac7, 0x45a8, { 0x8c, 0xf2, 0x95, 0x69, 0x83, 0xea, 0x18, 0xf3 } };
    };

    class layout_base;

    template <>
    struct type_guid<layout_base>
    {
        static constexpr guid value{ 0x11aee394, 0x31f8, 0x40ac, { 0xb3, 0x8f, 0x69, 0xb8, 0x86, 0x36, 0x68, 0x22 } };
    };

    class layout_base : public multicontainer
    {
    public:
        META_CLASS_IMPL(multicontainer)

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
