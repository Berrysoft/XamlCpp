#ifndef XAML_CMDLINE_DESERIALIZER_HPP
#define XAML_CMDLINE_DESERIALIZER_HPP

#include <xaml/cmdline/parser.hpp>
#include <xaml/meta/meta.hpp>

namespace xaml::cmdline
{
    XAML_CMDLINE_API std::shared_ptr<xaml::meta_class> deserialize(xaml::reflection_info const* refl, options nodes);
} // namespace xaml::cmdline

#endif // !XAML_CMDLINE_DESERIALIZER_HPP