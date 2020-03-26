#include <xaml/cmdline/deserializer.hpp>

using namespace std;

namespace xaml::cmdline
{
    shared_ptr<meta_class> deserializer::deserialize(reflection_info const* refl, options nodes)
    {
        shared_ptr<meta_class> result{ refl->construct() };
        for (auto& prop : nodes.properties)
        {
            if (prop.info && prop.info->can_write())
            {
                prop.info->set(result.get(), prop.value);
            }
        }
        for (auto& [name, prop] : nodes.collection_properties)
        {
            if (prop.info && prop.info->can_add())
            {
                for (auto& v : prop.values)
                {
                    prop.info->add(result.get(), v);
                }
            }
        }
        return result;
    }
} // namespace xaml::cmdline
