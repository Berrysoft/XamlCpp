#ifndef XAML_META_ENUM_META_HPP
#define XAML_META_ENUM_META_HPP

#include <unordered_map>
#include <xaml/meta/conv.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/strings.hpp>

namespace xaml
{
    template <typename TEnum, typename TChar>
    struct enum_meta
    {
        constexpr TEnum operator()(std::basic_string_view<TChar> str) const noexcept { return {}; }
        std::basic_string_view<TChar> operator()(TEnum e) const noexcept { return {}; }
    };

    template <typename TKey1, typename TKey2, typename Hash1 = std::hash<TKey1>, typename Hash2 = std::hash<TKey2>>
    class __unordered_bimap
    {
    private:
        std::unordered_map<std::size_t, TKey1> m_map1{};
        std::unordered_map<std::size_t, TKey2> m_map2{};

        Hash1 m_hasher1{};
        Hash2 m_hasher2{};

    public:
        __unordered_bimap() noexcept {}
        __unordered_bimap(std::initializer_list<std::pair<TKey1, TKey2>> pairs) noexcept : __unordered_bimap()
        {
            for (auto& p : pairs)
            {
                insert_or_assign(p);
            }
        }

        void insert_or_assign(std::pair<TKey1, TKey2> const& pair) noexcept
        {
            m_map1[m_hasher2(pair.second)] = pair.first;
            m_map2[m_hasher1(pair.first)] = pair.second;
        }

        TKey2& at1(TKey1 const& key) noexcept { return m_map2[m_hasher1(key)]; }
        TKey1& at2(TKey2 const& key) noexcept { return m_map1[m_hasher2(key)]; }
    };

    STRING_CONST(__namespace_delimeter, "::")

    template <typename TEnum, typename TChar, __unordered_bimap<std::basic_string_view<TChar>, TEnum>* pmap>
    struct __enum_meta_helper
    {
        inline TEnum operator()(std::basic_string_view<TChar> str) const noexcept
        {
            return pmap->at1(str);
        }

        inline std::basic_string<TChar> operator()(TEnum e) const noexcept
        {
            auto [ns, name] = *get_type_name(std::type_index(typeid(TEnum)));
            std::basic_ostringstream<TChar> stream;
            stream << __namespace_delimeter<TChar> << ns << __namespace_delimeter<TChar> << name << __namespace_delimeter<TChar> << pmap->at2(e);
            return stream.str();
        }
    };

    template <typename TEnum, typename TChar, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
    inline TEnum __stoenum(std::basic_string_view<TChar> str)
    {
        return enum_meta<TEnum, TChar>{}(str);
    }

    template <typename TEnum>
    inline TEnum stoenum(std::string_view str)
    {
        return __stoenum<TEnum, char>(str);
    }

    template <typename TEnum>
    inline TEnum stoenum(std::wstring_view str)
    {
        return __stoenum<TEnum, wchar_t>(str);
    }

    template <typename TEnum>
    struct value_converter_traits<TEnum, std::enable_if_t<std::is_enum_v<TEnum>>> : __value_converter_traits_helper<TEnum, __stoenum<TEnum, char>, __stoenum<TEnum, wchar_t>>
    {
    };
} // namespace xaml

#endif // !XAML_META_ENUM_META_HPP
