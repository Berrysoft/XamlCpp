#ifndef XAML_META_DEFAULT_PROPERTY_HPP
#define XAML_META_DEFAULT_PROPERTY_HPP

#include <xaml/meta/meta_macro.hpp>

namespace xaml
{
    class default_property;

    template <>
    struct type_guid<default_property>
    {
        static constexpr guid value{ 0xaf8c4c53, 0xa4a0, 0x4f46, { 0xbd, 0x28, 0x2a, 0xf6, 0xb2, 0xfe, 0xe1, 0x79 } };
    };

    class default_property : public meta_class
    {
    public:
        META_CLASS_IMPL(meta_class)

    private:
        std::string m_property_name;

    public:
        std::string_view get_property_name() const noexcept { return m_property_name; }
        void set_property_name(std::string_view value) { m_property_name = value; }

    public:
        default_property(std::string_view prop_name = {}) : m_property_name(prop_name) {}
        ~default_property() override {}

        REGISTER_CLASS_DECL(xaml, default_property, "xaml/meta")
        {
            ADD_CTOR();
            ADD_CTOR(std::string_view);
            ADD_PROP(property_name);
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_META_DEFAULT_PROPERTY_HPP
