#ifndef XAML_UI_CONTROLS_PROGRESS_HPP
#define XAML_UI_CONTROLS_PROGRESS_HPP

#include <xaml/ui/control.hpp>

namespace xaml
{
    class progress : public control
    {
    public:
        XAML_UI_CONTROLS_API progress();
        XAML_UI_CONTROLS_API ~progress();

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API void __size_to_fit() override;
#endif // XAML_UI_WINDOWS

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_CONTROLS_API virtual void draw_progress();

    public:
        EVENT(value_changed, progress&, std::size_t)
        PROP_CONSTEXPR_EVENT(value, std::size_t)

        PROP_CONSTEXPR(minimum, std::size_t)
        PROP_CONSTEXPR(maximum, std::size_t)

    public:
#define ADD_PROGRESS_MEMBERS() \
    ADD_CONTROL_MEMBERS();     \
    ADD_PROP_EVENT(value);     \
    ADD_PROP(minimum);         \
    ADD_PROP(maximum)

        REGISTER_CLASS_DECL(xaml, progress, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_PROGRESS_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_PROGRESS_HPP
