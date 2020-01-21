#include <map>
#include <xaml/meta/binding.hpp>
#include <xaml/meta/event.hpp>

using namespace std;

namespace xaml
{
    static string get_property_changed_event_name(string_view name)
    {
        return (string)name + "_changed";
    }

    class __binding_guard
    {
    private:
        using token_type = typename event<>::token_type;

        shared_ptr<meta_class> target;
        shared_ptr<meta_class> source;

        property_info target_prop;
        event_info target_event;
        property_info source_prop;
        event_info source_event;

        token_type target_token;
        token_type source_token;

    public:
        __binding_guard(shared_ptr<meta_class> target, string_view target_prop, shared_ptr<meta_class> source, string_view source_prop, binding_mode mode = binding_mode::one_time)
            : target(target), source(source),
              target_prop(get_property(target->this_type(), target_prop)), target_event(get_event(target->this_type(), get_property_changed_event_name(target_prop))),
              source_prop(get_property(source->this_type(), source_prop)), source_event(get_event(source->this_type(), get_property_changed_event_name(source_prop)))
        {
            this->target_prop.set(target, this->source_prop.get(source));
            if (mode & binding_mode::one_way)
            {
                source_token = source_event.add(source, function<void()>([this]() -> void { this->target_prop.set(this->target, this->source_prop.get(this->source)); }));
            }
            if (mode & binding_mode::one_way_to_source)
            {
                target_token = target_event.add(target, function<void()>([this]() -> void { this->source_prop.set(this->source, this->target_prop.get(this->target)); }));
            }
        }

        ~__binding_guard()
        {
            source_event.remove(source, source_token);
            target_event.remove(target, target_token);
        }
    };

    static map<size_t, shared_ptr<__binding_guard>> bind_map;
    static size_t bind_index = 0;

    size_t bind(shared_ptr<meta_class> target, string_view target_prop, shared_ptr<meta_class> source, string_view source_prop, binding_mode mode)
    {
        bind_index++;
        bind_map.emplace(bind_index, make_shared<__binding_guard>(target, target_prop, source, source_prop, mode));
        return bind_index;
    }

    void unbind(size_t token)
    {
        bind_map.erase(token);
    }
} // namespace xaml
