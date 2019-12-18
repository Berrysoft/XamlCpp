#ifndef XAML_UI_CONTAINER_HPP
#define XAML_UI_CONTAINER_HPP

#include <map>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class container : public control
    {
#ifdef XAML_UI_WINDOWS
    protected:
        std::map<HWND, std::shared_ptr<control>> _children;
#elif defined(XAML_UI_LINUX)
    protected:
        std::map<GtkContainer*, std::shared_ptr<control>> _children;
#endif // XAML_UI_WINDOWS

    public:
        container();
        virtual ~container() override;

        void add_children(std::shared_ptr<control> const& child);
        void remove_children(std::shared_ptr<control> const& child);
    };
} // namespace xaml

#endif // !XAML_UI_CONTAINER_HPP
