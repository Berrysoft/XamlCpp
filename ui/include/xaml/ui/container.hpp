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
        container();
        virtual ~container() override;

        bool is_container() const override final { return true; }
        bool is_multicontainer() const override final { return false; }

        std::shared_ptr<control> get_child() const noexcept { return m_child; }
        void set_child(std::shared_ptr<control> const& value)
        {
            if (m_child != value)
            {
                m_child = value;
                m_child->set_parent(std::reinterpret_pointer_cast<container>(shared_from_this()));
            }
        }
    };

    class multicontainer : public control
    {
    protected:
        std::vector<std::shared_ptr<control>> m_children{};

    public:
        multicontainer();
        virtual ~multicontainer() override;

        bool is_container() const override final { return true; }
        bool is_multicontainer() const override final { return true; }

        void add_child(std::shared_ptr<control> const& child);
        void remove_child(std::shared_ptr<control> const& child);
    };
} // namespace xaml

#endif // !XAML_UI_CONTAINER_HPP
