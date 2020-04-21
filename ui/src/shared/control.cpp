#include <shared/control.hpp>
#include <xaml/ui/control.h>

using namespace std;

//#include <xaml/ui/container.hpp>
//#include <xaml/ui/control.hpp>
//
//using namespace std;
//
//namespace xaml
//{
//    control::control() : meta_class(), m_is_visible(true)
//    {
//        add_size_changed([this](shared_ptr<control>, size) {
//            if (get_handle())
//            {
//                draw_size();
//                __parent_redraw();
//            }
//        });
//        add_is_visible_changed([this](shared_ptr<control>, bool) { if (get_handle()) draw_visible(); });
//    }
//
//    control::~control() {}
//
//    void control::set_parent(weak_ptr<control> value)
//    {
//        auto sparent = m_parent.lock();
//        auto svalue = value.lock();
//        if (sparent != svalue)
//        {
//            if (sparent)
//            {
//                if (auto multic = sparent->query<multicontainer>())
//                {
//                    multic->remove_child(shared_from_this<control>());
//                }
//                else if (auto c = sparent->query<container>())
//                {
//                    c->set_child(nullptr);
//                }
//            }
//            if (svalue)
//            {
//                if (auto multic = svalue->query<multicontainer>())
//                {
//                    multic->add_child(shared_from_this<control>());
//                }
//                else if (auto c = svalue->query<container>())
//                {
//                    c->set_child(shared_from_this<control>());
//                }
//            }
//            m_parent = svalue;
//            m_parent_changed(*this, *svalue.get());
//        }
//    }
//
//    void control::__parent_redraw()
//    {
//        if (auto sparent = get_parent().lock())
//            sparent->__parent_redraw();
//    }
//} // namespace xaml
