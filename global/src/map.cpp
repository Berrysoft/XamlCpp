#include <xaml/map.h>

using namespace std;

struct xaml_key_value_pair_impl : xaml_implement<xaml_key_value_pair_impl, xaml_key_value_pair, xaml_object>
{
private:
    xaml_ptr<xaml_object> m_key;
    xaml_ptr<xaml_object> m_value;

public:
    xaml_key_value_pair_impl(xaml_ptr<xaml_object>&& key, xaml_ptr<xaml_object>&& value) noexcept : m_key(move(key)), m_value(move(value)) {}

    xaml_result XAML_CALL get_key(xaml_object** ptr) noexcept
    {
        m_key->add_ref();
        *ptr = m_key.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_value(xaml_object** ptr) noexcept
    {
        m_value->add_ref();
        *ptr = m_value.get();
        return XAML_S_OK;
    }
};

xaml_result xaml_key_value_pair_new(xaml_object* key, xaml_object* value, xaml_key_value_pair** ptr) noexcept
{
    return xaml_object_new<xaml_key_value_pair_impl>(ptr, key, value);
}

struct xaml_map_enumerator_impl : xaml_implement<xaml_map_enumerator_impl, xaml_enumerator, xaml_object>
{
private:
    unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>>::const_iterator m_begin, m_end;
    bool m_init;

public:
    xaml_map_enumerator_impl(unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>>::const_iterator begin, unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>>::const_iterator end) noexcept
        : m_begin(begin), m_end(end), m_init(false) {}

    xaml_result XAML_CALL move_next(bool* pb) noexcept override
    {
        if (!m_init)
        {
            m_init = true;
        }
        else
        {
            ++m_begin;
        }
        *pb = m_begin != m_end;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_current(xaml_object** ptr) noexcept override
    {
        if (m_begin == m_end) return XAML_E_FAIL;
        xaml_ptr<xaml_key_value_pair> pair;
        XAML_RETURN_IF_FAILED(xaml_key_value_pair_new(m_begin->first.get(), m_begin->second.get(), &pair));
        *ptr = pair.get();
        return XAML_S_OK;
    }
};

struct xaml_map_impl : xaml_implement<xaml_map_impl, xaml_map, xaml_map_view, xaml_enumerable, xaml_object>
{
private:
    unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>> m_map{};

public:
    xaml_map_impl() noexcept {}
    xaml_map_impl(unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>>&& map) noexcept : m_map(move(map)) {}

    xaml_result XAML_CALL get_size(size_t* psize) noexcept override
    {
        *psize = m_map.size();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL lookup(xaml_object* key, xaml_object** ptr) noexcept override
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) return XAML_E_KEYNOTFOUND;
        it->second->add_ref();
        *ptr = it->second.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL has_key(xaml_object* key, bool* pb) noexcept override
    {
        *pb = m_map.contains(key);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL insert(xaml_object* key, xaml_object* value, bool* pb) noexcept override
    {
        auto [it, inserted] = m_map.emplace(key, value);
        if (!inserted)
        {
            it->second = nullptr;
            it->second = value;
        }
        *pb = !inserted;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove(xaml_object* key) noexcept override
    {
        m_map.erase(key);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        m_map.clear();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator** ptr) noexcept override
    {
        return xaml_object_new<xaml_map_enumerator_impl>(ptr, m_map.begin(), m_map.end());
    }
};

xaml_result xaml_map_new(unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>>&& map, xaml_map** ptr) noexcept
{
    return xaml_object_new<xaml_map_impl>(ptr, move(map));
}

xaml_result xaml_map_new(xaml_map** ptr) noexcept
{
    return xaml_map_new({}, ptr);
}
