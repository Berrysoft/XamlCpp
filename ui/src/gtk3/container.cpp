#include <xaml/ui/container.hpp>

using namespace std;

namespace xaml
{
    container::container() : control() {}

    container::~container() {}

    void container::add_children(shared_ptr<control> const& child)
    {
        if (child)
        {
            child->set_parent(reinterpret_pointer_cast<container>(shared_from_this()));
            _children.emplace(GTK_CONTAINER(child->get_handle()), child);
        }
    }

    void container::remove_children(shared_ptr<control> const& child)
    {
        child->set_parent(nullptr);
        _children.erase(GTK_CONTAINER(child->get_handle()));
    }
} // namespace xaml
