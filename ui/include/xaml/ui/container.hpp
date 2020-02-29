#ifndef XAML_UI_CONTAINER_HPP
#define XAML_UI_CONTAINER_HPP

#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class container : public control
    {
    private:
        std::shared_ptr<control> m_child{ nullptr };

    public:
        container() : control() {}
        virtual ~container() override {}

        EVENT(child_changed, container&, std::shared_ptr<control>)

    public:
        std::shared_ptr<control> get_child() const noexcept { return m_child; }
        void set_child(std::shared_ptr<control> const& value)
        {
            if (m_child != value)
            {
                m_child = value;
                m_child->set_parent(std::static_pointer_cast<control>(shared_from_this()));
                m_child_changed(*this, value);
            }
        }

    public:
#define ADD_CONTAINER_MEMBERS() \
    ADD_CONTROL_MEMBERS();      \
    ADD_PROP(child);            \
    ADD_EVENT(child_changed);   \
    ADD_DEF_PROP(child)

        REGISTER_CLASS_DECL(xaml, container)
        {
            ADD_CONTAINER_MEMBERS();
        }
        REGISTER_CLASS_END()
    };

    class multicontainer : public control
    {
    protected:
        std::vector<std::shared_ptr<control>> m_children{};

    public:
        multicontainer() : control() {}
        virtual ~multicontainer() override {}

        XAML_UI_API void add_child(std::shared_ptr<control> const& child);
        XAML_UI_API void remove_child(std::shared_ptr<control> const& child);
        array_view<std::shared_ptr<control>> get_children() const noexcept { return m_children; }

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

    public:
#define ADD_MULTICONTAINER_MEMBERS()                      \
    ADD_CONTROL_MEMBERS();                                \
    ADD_COLLECTION_PROP(child, std::shared_ptr<control>); \
    ADD_DEF_PROP(child)

        REGISTER_CLASS_DECL(xaml, multicontainer)
        {
            ADD_MULTICONTAINER_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTAINER_HPP
