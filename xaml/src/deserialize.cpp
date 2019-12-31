#include <xaml/deserialize.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    void deserializer::open(string_view file)
    {
        reader.open(file);
    }

    void deserializer::deserialize(std::shared_ptr<meta_class> mc)
    {
        if (mc)
        {
            __xaml_node root_node = reader.parse();
            for (auto& prop : root_node.properties)
            {
                prop.info.set(mc, prop.value);
            }
            for (auto& ev : root_node.events)
            {
                // TODO
            }
            for (auto& prop : root_node.properties)
            {
                // TODO
            }
            for (auto& node : root_node.children)
            {
                // TODO
            }
        }
    }
} // namespace xaml
