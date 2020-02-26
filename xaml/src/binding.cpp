#include <xaml/markup/binding.hpp>
#include <xaml/meta/binding.hpp>

using namespace std;

namespace xaml
{
    void binding::provide(meta_context& ctx, markup_context& context)
    {
        xaml::bind(ctx, context.current_element(), context.current_property(), context.find_element(m_element), m_path, m_mode);
    }
} // namespace xaml
