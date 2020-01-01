#ifndef XAML_BINDING_EXTENSION_HPP
#define XAML_BINDING_EXTENSION_HPP

#include <xaml/markup_extension.hpp>
#include <xaml/meta/binding.hpp>

namespace xaml
{
    class binding_extension : public markup_extension, public meta_class_impl<binding_extension>
    {
        PROP(element, std::string_view)
        PROP(path, std::string_view)
        PROP(mode, binding_mode)

    public:
        ~binding_extension() override {}

        void provide(markup_context& context) override;

#define ADD_BINDING_EXTENSION_MEMBERS() \
    ADD_MARKUP_EXTENSION_MEMBERS();     \
    ADD_PROP(element);                  \
    ADD_PROP(path);                     \
    ADD_PROP(mode)

    public:
        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, binding_extension);
            ADD_CTOR_DEF();
            ADD_BINDING_EXTENSION_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_BINDING_EXTENSION_HPP
