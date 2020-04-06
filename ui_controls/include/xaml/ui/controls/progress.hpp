#ifndef XAML_UI_CONTROLS_PROGRESS_HPP
#define XAML_UI_CONTROLS_PROGRESS_HPP

#include <xaml/ui/control.hpp>

namespace xaml
{
    class progress;

    template <>
    struct type_guid<progress>
    {
        static constexpr guid value{ 0x505267df, 0x2287, 0x4c64, { 0xab, 0xf2, 0xb2, 0xea, 0xcf, 0xde, 0x8a, 0xe1 } };
    };

    class progress : public control
    {
    public:
        META_CLASS_IMPL(control)

    public:
        XAML_UI_CONTROLS_API progress();
        XAML_UI_CONTROLS_API ~progress();

#ifndef XAML_UI_GTK3
    public:
        XAML_UI_CONTROLS_API void __size_to_fit() override;
#endif // !XAML_UI_GTK3

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_CONTROLS_API virtual void draw_progress();
        XAML_UI_CONTROLS_API virtual void draw_indeterminate();

    public:
        EVENT(value_changed, std::shared_ptr<progress>, std::size_t)
        PROP_CONSTEXPR_EVENT(value, std::size_t)

        PROP_CONSTEXPR(minimum, std::size_t)
        PROP_CONSTEXPR(maximum, std::size_t)

        EVENT(is_indeterminate_changed, std::shared_ptr<progress>, bool)
        PROP_CONSTEXPR_EVENT(is_indeterminate, bool)

    public:
#define ADD_PROGRESS_MEMBERS() \
    ADD_CONTROL_MEMBERS();     \
    ADD_PROP_EVENT(value);     \
    ADD_PROP(minimum);         \
    ADD_PROP(maximum)

        REGISTER_CLASS_DECL(xaml, progress, "xaml/ui/controls")
        {
            ADD_CTOR();
            ADD_PROGRESS_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_PROGRESS_HPP
