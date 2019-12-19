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
            if (_children.find(child->get_handle()) == _children.end())
            {
                _children.emplace(child->get_handle(), child);
                child->set_parent(reinterpret_pointer_cast<container>(shared_from_this()));
            }
        }
    }

    void container::remove_children(shared_ptr<control> const& child)
    {
        _children.erase(child->get_handle());
        child->set_parent(nullptr);
    }
} // namespace xaml
