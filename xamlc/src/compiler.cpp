#include <ostream>
#include <xaml/compiler.hpp>

using namespace std;

namespace xaml
{
    ostream& compiler::write_type(ostream& stream, type_index type)
    {
        auto t = *get_type_name(type);
        return stream << "::" << get<0>(t) << "::" << get<1>(t);
    }

    ostream& compiler::write_construct(ostream& stream, xaml_node& node)
    {
        return write_type(stream << "auto " << node.name << " = ::xaml::construct<", node.type) << ">();" << endl;
    }

    ostream& compiler::compile(ostream& stream)
    {
        return stream;
    }
} // namespace xaml
