#ifndef XAML_UI_CONTROLS_LAYOUT_BASE_HPP
#define XAML_UI_CONTROLS_LAYOUT_BASE_HPP

#include <xaml/ui/container.hpp>

namespace xaml
{
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
        virtual void __draw_impl(rectangle const& region, std::function<void(std::shared_ptr<control>, rectangle const&)> func = {}) = 0;

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    public:
#define ADD_LAYOUT_BASE_MEMBERS() ADD_MULTICONTAINER_MEMBERS()

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, layout_base);
            ADD_LAYOUT_BASE_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_LAYOUT_BASE_HPP
