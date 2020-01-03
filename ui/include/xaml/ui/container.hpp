#ifndef XAML_UI_CONTAINER_HPP
#define XAML_UI_CONTAINER_HPP

#include <vector>
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

        bool is_container() const override final { return true; }
        bool is_multicontainer() const override final { return false; }

        EVENT(child_changed, container&, std::shared_ptr<control>)

    public:
        std::shared_ptr<control> get_child() const noexcept { return m_child; }
        void set_child(std::shared_ptr<control> const& value)
        {
            if (m_child != value)
            {
                m_child = value;
                m_child->set_parent(std::static_pointer_cast<container>(shared_from_this()));
                m_child_changed(*this, value);
            }
        }

    public:
#define ADD_CONTAINER_MEMBERS() \
    ADD_CONTROL_MEMBERS();      \
    ADD_PROP(child);            \
    ADD_EVENT(child_changed)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, container);
            ADD_CONTAINER_MEMBERS();
        }
    };

    class multicontainer : public control
    {
    protected:
        std::vector<std::shared_ptr<control>> m_children{};

    public:
        multicontainer() : control() {}
        virtual ~multicontainer() override {}

        bool is_container() const override final { return true; }
        bool is_multicontainer() const override final { return true; }

        XAML_API void add_child(std::shared_ptr<control> const& child);
        XAML_API void remove_child(std::shared_ptr<control> const& child);
        std::vector<std::shared_ptr<control>> const& get_children() const noexcept { return m_children; }

    public:
#define ADD_MULTICONTAINER_MEMBERS() \
    ADD_CONTROL_MEMBERS();           \
    ADD_METHOD(add_child);           \
    ADD_METHOD(remove_child)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, multicontainer);
            ADD_MULTICONTAINER_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTAINER_HPP
