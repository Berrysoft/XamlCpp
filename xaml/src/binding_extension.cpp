#include <xaml/binding_extension.hpp>
#include <xaml/meta/binding.hpp>

using namespace std;

namespace xaml
{
    XAML_API void binding_extension::provide(markup_context& context)
    {
        xaml::bind(context.current_element(), context.current_property(), context.find_element(m_element), m_path, m_mode);
    }
} // namespace xaml
