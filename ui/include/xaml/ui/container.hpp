#ifndef XAML_UI_CONTAINER_HPP
#define XAML_UI_CONTAINER_HPP

#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class container;

    template <>
    struct type_guid<container>
    {
        static constexpr guid value{ 0x211a9bce, 0xc31a, 0x42ab, 0x8f, 0x4e, 0x2d, 0x45, 0x0f, 0xb2, 0xc3, 0xa9 };
    };

    class container : public control
    {
    public:
        META_CLASS_IMPL(control)

    private:
        std::shared_ptr<control> m_child{ nullptr };

    public:
        container() : control() {}
        virtual ~container() override {}

        EVENT(child_changed, std::shared_ptr<container>, std::shared_ptr<control>)

    public:
        std::shared_ptr<control> get_child() const noexcept { return m_child; }
        void set_child(std::shared_ptr<control> const& value)
        {
            if (m_child != value)
            {
                m_child = value;
                m_child->set_parent(std::static_pointer_cast<control>(shared_from_this()));
                m_child_changed(std::static_pointer_cast<container>(shared_from_this()), value);
            }
        }

    public:
#define ADD_CONTAINER_MEMBERS() \
    ADD_CONTROL_MEMBERS();      \
    ADD_PROP(child);            \
    ADD_EVENT(child_changed);   \
    ADD_DEF_PROP(child)

        REGISTER_CLASS_DECL(xaml, container, "xaml/ui")
        {
            ADD_CONTAINER_MEMBERS();
        }
        REGISTER_CLASS_END()
    };

    class multicontainer;

    template <>
    struct type_guid<multicontainer>
    {
        static constexpr guid value{ 0xc9a53763, 0x404b, 0x4531, 0xba, 0x56, 0x56, 0x01, 0xc9, 0x49, 0x05, 0x8d };
    };

    class multicontainer : public control
    {
    public:
        META_CLASS_IMPL(control)

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
        XAML_UI_API std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

    public:
#define ADD_MULTICONTAINER_MEMBERS()                      \
    ADD_CONTROL_MEMBERS();                                \
    ADD_COLLECTION_PROP(child, std::shared_ptr<control>); \
    ADD_DEF_PROP(child)

        REGISTER_CLASS_DECL_FILE(xaml, multicontainer, "xaml/ui/container.hpp")
        {
            ADD_MULTICONTAINER_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTAINER_HPP
