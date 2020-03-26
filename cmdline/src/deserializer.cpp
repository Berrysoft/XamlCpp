#include <xaml/cmdline/deserializer.hpp>

using namespace std;

namespace xaml::cmdline
{
    unique_ptr<meta_class> deserializer::deserialize(reflection_info const* refl, array_view<option_node> nodes)
    {
        unique_ptr<meta_class> result{ refl->construct() };
        for (auto& node : nodes)
        {
            if (node.info && node.info->can_write())
            {
                node.info->set(result.get(), node.value);
            }
        }
        return result;
    }
} // namespace xaml::cmdline
