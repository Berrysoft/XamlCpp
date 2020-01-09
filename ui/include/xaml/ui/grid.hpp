#ifndef XAML_UI_GRID_HPP
#define XAML_UI_GRID_HPP

#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <xaml/meta/conv.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/drawing.hpp>

namespace xaml
{
    enum class grid_layout
    {
        abs,
        star,
        compact
    };

    struct grid_length
    {
        double value;
        grid_layout layout;
    };

    struct grid_index
    {
        std::size_t column, row;
    };

    CHAR_CONST(__star, '*')

    STRING_CONST(__grid_length_abs, "abs")
    STRING_CONST(__grid_length_star, "star")
    STRING_CONST(__grid_length_auto, "auto")
    STRING_CONST(__grid_length_compact, "compact")

    template <typename TChar>
    struct __grid_length_enum_meta_helper
    {
        inline static __unordered_bimap<std::basic_string_view<TChar>, grid_length> enum_map{
            { __grid_length_abs<TChar>, grid_length::abs },
            { __grid_length_star<TChar>, grid_length::star },
            { __grid_length_compact<TChar>, grid_length::compact }
        };
    };

    template <typename TChar>
    struct enum_meta<grid_length, TChar> : __enum_meta_helper<grid_length, TChar, &__grid_length_enum_meta_helper<TChar>::enum_map>
    {
    };

    template <typename TChar>
    inline std::vector<grid_length> __stogls(std::basic_string_view<TChar> str)
    {
        std::vector<grid_length> result;
        std::size_t offset = 0;
        do
        {
            std::size_t index = str.find_first_of(__delimeter<TChar>, offset);
            std::basic_string_view<TChar> lenstr = str.substr(offset, index - offset);
            if (lenstr == __grid_length_auto<TChar> || lenstr == __grid_length_compact<TChar>)
            {
                result.push_back({ 0, grid_layout::compact });
            }
            else if (lenstr.back() == __star<TChar>)
            {
                double rate = stof<double>(lenstr.substr(0, lenstr.length() - 1));
                result.push_back({ rate, grid_layout::star });
            }
            else
            {
                double value = stof<double>(lenstr);
                result.push_back({ value, grid_layout::abs });
            }
            offset = str.find_first_not_of(__delimeter<TChar>, index);
        } while (offset != std::basic_string_view<TChar>::npos);
        return result;
    }

    inline std::vector<grid_length> stogls(std::string_view str) { return __stogls<char>(str); }
    inline std::vector<grid_length> stogls(std::wstring_view str) { return __stogls<wchar_t>(str); }

    template <>
    struct value_converter_traits<std::vector<grid_length> const&, void> : __value_converter_traits_helper<std::vector<grid_length>, __stogls<char>, __stogls<wchar_t>>
    {
    };

    class grid : public multicontainer
    {
    public:
        XAML_API grid();
        XAML_API ~grid() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#if defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
    private:
        std::unordered_map<std::shared_ptr<control>, bool> m_put_map{};
#endif // XAML_UI_GTK3

    public:
        XAML_API void __draw(rectangle const& region) override;

    private:
        std::vector<grid_length> m_columns;
        std::vector<grid_length> m_rows;

    public:
        void add_column(grid_length length) { m_columns.push_back(std::move(length)); }
        void add_row(grid_length length) { m_rows.push_back(std::move(length)); }

        std::vector<grid_length> const& get_columns() const noexcept { return m_columns; }
        void set_columns(std::vector<grid_length> const& value) { m_columns = value; }

        std::vector<grid_length> const& get_rows() const noexcept { return m_rows; }
        void set_rows(std::vector<grid_length> const& value) { m_rows = value; }

    private:
        XAML_API static std::unordered_map<std::shared_ptr<control>, grid_index> m_indecies;

    public:
        static void set_column(std::shared_ptr<control> const& c, std::size_t col) { m_indecies[c].column = col; }
        static std::size_t get_column(std::shared_ptr<control> const& c) { return m_indecies[c].column; }
        static void set_row(std::shared_ptr<control> const& c, std::size_t row) { m_indecies[c].row = row; }
        static std::size_t get_row(std::shared_ptr<control> const& c) { return m_indecies[c].row; }

    public:
#define ADD_GRID_MEMBERS()                             \
    ADD_MULTICONTAINER_MEMBERS();                      \
    ADD_PROP(columns);                                 \
    ADD_PROP(rows);                                    \
    ADD_ATTACH_PROP(column, std::shared_ptr<control>); \
    ADD_ATTACH_PROP(row, std::shared_ptr<control>)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, grid);
            ADD_CTOR_DEF();
            ADD_GRID_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_GRID_HPP
