#include <map>
#include <xaml/meta/binding.hpp>
#include <xaml/meta/event.hpp>

using namespace std;

namespace xaml
{
    class __binding_guard
    {
    private:
        using token_type = typename event<>::token_type;

        std::weak_ptr<meta_class> target;
        std::weak_ptr<meta_class> source;

        property_info target_prop;
        event_info target_event;
        property_info source_prop;
        event_info source_event;

        token_type target_token;
        token_type source_token;

    public:
        __binding_guard(weak_ptr<meta_class> target, string_view target_prop, weak_ptr<meta_class> source, string_view source_prop, binding_mode mode = binding_mode::one_time)
            : target(target), source(source)
        {
            auto starget = target.lock();
            auto ssource = source.lock();
            this->target_prop = get_property(target.lock()->this_type(), target_prop);
            target_event = get_event(target.lock()->this_type(), __get_property_changed_event_name(target_prop));
            this->source_prop = get_property(source.lock()->this_type(), source_prop);
            source_event = get_event(source.lock()->this_type(), __get_property_changed_event_name(source_prop));
            this->target_prop.set(starget.get(), this->source_prop.get(ssource.get()));
            if (mode & binding_mode::one_way)
            {
                source_token = source_event.add(
                    ssource.get(),
                    function<void()>(
                        [this]() -> void {
                            if (auto ssource = this->source.lock())
                                if (auto starget = this->target.lock())
                                    this->target_prop.set(starget.get(), this->source_prop.get(ssource.get()));
                        }));
            }
            if (mode & binding_mode::one_way_to_source)
            {
                target_token = target_event.add(
                    starget.get(),
                    function<void()>(
                        [this]() -> void {
                            if (auto ssource = this->source.lock())
                                if (auto starget = this->target.lock())
                                    this->source_prop.set(ssource.get(), this->target_prop.get(starget.get()));
                        }));
            }
        }

        ~__binding_guard()
        {
            if (auto ssource = source.lock())
                source_event.remove(ssource.get(), source_token);
            if (auto starget = target.lock())
                target_event.remove(starget.get(), target_token);
        }
    };

    static map<size_t, unique_ptr<__binding_guard>> bind_map;
    static size_t bind_index = 0;

    size_t bind(weak_ptr<meta_class> target, string_view target_prop, weak_ptr<meta_class> source, string_view source_prop, binding_mode mode)
    {
        bind_index++;
        bind_map.emplace(bind_index, make_unique<__binding_guard>(target, target_prop, source, source_prop, mode));
        return bind_index;
    }

    void unbind(size_t token)
    {
        bind_map.erase(token);
    }
} // namespace xaml
