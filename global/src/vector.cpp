#include <xaml/object.hpp>
#include <xaml/vector.hpp>

using namespace std;

struct xaml_vector_enumerator_impl : xaml_implement<xaml_vector_enumerator_impl, xaml_enumerator, xaml_object>
{
private:
    vector<xaml_ptr<xaml_object>>::const_iterator m_begin, m_end;

public:
    xaml_vector_enumerator_impl(vector<xaml_ptr<xaml_object>>::const_iterator begin, vector<xaml_ptr<xaml_object>>::const_iterator end) noexcept
        : m_begin(begin), m_end(end) {}

    xaml_result XAML_CALL move_next() noexcept override
    {
        if (m_begin == m_end) return XAML_E_FAIL;
        ++m_begin;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_current(xaml_object** ptr) const noexcept override
    {
        if (m_begin == m_end) return XAML_E_FAIL;
        (*m_begin)->add_ref();
        *ptr = m_begin->get();
        return XAML_S_OK;
    }
};

struct xaml_vector_impl : xaml_implement<xaml_vector_impl, xaml_vector, xaml_vector_view, xaml_object>
{
private:
    vector<xaml_ptr<xaml_object>> m_vec{};

public:
    xaml_vector_impl(vector<xaml_ptr<xaml_object>>&& vec) noexcept : m_vec(move(vec)) {}

    xaml_result XAML_CALL get_size(size_t* psize) const noexcept override
    {
        *psize = m_vec.size();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_at(size_t index, xaml_object** ptr) const noexcept override
    {
        if (index >= m_vec.size()) return XAML_E_INVALIDARG;
        auto& res = m_vec[index];
        *ptr = res.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_at(size_t index, xaml_object* obj) noexcept override
    {
        if (index >= m_vec.size()) return XAML_E_INVALIDARG;
        obj->add_ref();
        m_vec[index] = obj;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL append(xaml_object* obj) noexcept override
    {
        try
        {
            obj->add_ref();
            m_vec.push_back(obj);
            return XAML_S_OK;
        }
        catch (bad_alloc const&)
        {
            obj->release();
            return XAML_E_OUTOFMEMORY;
        }
        catch (...)
        {
            obj->release();
            return XAML_E_FAIL;
        }
    }

    xaml_result XAML_CALL remove_at(size_t index) noexcept override
    {
        if (index >= m_vec.size()) return XAML_E_INVALIDARG;
        m_vec.erase(m_vec.begin() + index);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator** ptr) const noexcept override
    {
        xaml_enumerator* res = new (nothrow) xaml_vector_enumerator_impl(m_vec.begin(), m_vec.end());
        if (!res) return XAML_E_OUTOFMEMORY;
        *ptr = res;
        return XAML_S_OK;
    }
};

xaml_result xaml_vector_new(vector<xaml_ptr<xaml_object>>&& vec, xaml_vector** ptr) noexcept
{
    return xaml_object_new<xaml_vector_impl>(ptr, move(vec));
}

xaml_result xaml_vector_new(xaml_vector** ptr) noexcept
{
    return xaml_vector_new({}, ptr);
}
