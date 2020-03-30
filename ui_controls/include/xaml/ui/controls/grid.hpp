#ifndef XAML_UI_GRID_HPP
#define XAML_UI_GRID_HPP

#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <xaml/meta/conv.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/controls/layout_base.hpp>
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
        std::size_t column_span, row_span;
    };

    CHAR_CONST(__star, '*')

    STRING_CONST(__grid_layout_abs, "abs")
    STRING_CONST(__grid_layout_star, "star")
    STRING_CONST(__grid_layout_auto, "auto")
    STRING_CONST(__grid_layout_compact, "compact")

    template <typename TChar>
    struct enum_meta<grid_layout, TChar> : __enum_meta_helper<enum_meta<grid_layout, TChar>>
    {
        typename __enum_meta_helper<enum_meta<grid_layout, TChar>>::map_type enum_map{
            { __grid_layout_abs<TChar>, grid_layout::abs },
            { __grid_layout_star<TChar>, grid_layout::star },
            { __grid_layout_compact<TChar>, grid_layout::compact }
        };
    };

    template <>
    struct type_guid<meta_box<grid_layout>>
    {
        static constexpr guid value{ 0x61521be2, 0x02fd, 0x40b8, { 0xb2, 0xa2, 0x5f, 0x60, 0xd9, 0x6b, 0x5c, 0x39 } };
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
            if (lenstr == __grid_layout_auto<TChar> || lenstr == __grid_layout_compact<TChar>)
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
    struct value_converter_traits<array_view<grid_length>, void> : __value_converter_traits_helper<std::vector<grid_length>, __stogls<char>, __stogls<wchar_t>>
    {
    };

    template <>
    struct type_guid<meta_box<std::vector<grid_length>>>
    {
        static constexpr guid value{ 0xfed37e9d, 0x1f3b, 0x4247, { 0x82, 0x69, 0x95, 0x56, 0x09, 0x93, 0xf3, 0x84 } };
    };

    template <>
    struct type_guid<meta_box<array_view<grid_length>>>
    {
        static constexpr guid value{ 0x51081ffe, 0xb917, 0x4e79, { 0x98, 0xb0, 0x9c, 0xee, 0x29, 0x40, 0x70, 0xe3 } };
    };

    class grid;

    template <>
    struct type_guid<grid>
    {
        static constexpr guid value{ 0x72737dd1, 0x7c10, 0x46e6, { 0x82, 0x38, 0x06, 0x2f, 0x43, 0x6b, 0xb0, 0x24 } };
    };

    class grid : public layout_base
    {
    public:
        META_CLASS_IMPL(layout_base)

    public:
        XAML_UI_CONTROLS_API grid();
        XAML_UI_CONTROLS_API ~grid() override;

    protected:
        XAML_UI_CONTROLS_API void __draw_impl(rectangle const& region, std::function<void(std::shared_ptr<control>, rectangle const&)> func = {}) override;

    private:
        std::vector<grid_length> m_columns;
        std::vector<grid_length> m_rows;

    public:
        void add_column(grid_length length) { m_columns.push_back(std::move(length)); }
        void add_row(grid_length length) { m_rows.push_back(std::move(length)); }

        array_view<grid_length> get_columns() const noexcept { return m_columns; }
        void set_columns(array_view<grid_length> value) { m_columns = value; }

        array_view<grid_length> get_rows() const noexcept { return m_rows; }
        void set_rows(array_view<grid_length> value) { m_rows = value; }

    private:
        XAML_UI_CONTROLS_API static std::unordered_map<control*, grid_index> m_indecies;

    public:
        static void set_column(control& c, std::size_t col) { m_indecies[&c].column = col; }
        static std::size_t get_column(control& c) { return m_indecies[&c].column; }
        static void set_row(control& c, std::size_t row) { m_indecies[&c].row = row; }
        static std::size_t get_row(control& c) { return m_indecies[&c].row; }
        static void set_column_span(control& c, std::size_t cs) { m_indecies[&c].column_span = cs; }
        static std::size_t get_column_span(control& c) { return m_indecies[&c].column_span; }
        static void set_row_span(control& c, std::size_t rs) { m_indecies[&c].row_span = rs; }
        static std::size_t get_row_span(control& c) { return m_indecies[&c].row_span; }

    public:
#define ADD_GRID_MEMBERS()                  \
    ADD_LAYOUT_BASE_MEMBERS();              \
    ADD_PROP(columns);                      \
    ADD_PROP(rows);                         \
    ADD_ATTACH_PROP(column, control&);      \
    ADD_ATTACH_PROP(row, control&);         \
    ADD_ATTACH_PROP(column_span, control&); \
    ADD_ATTACH_PROP(row_span, control&)

        REGISTER_CLASS_DECL(xaml, grid, "xaml/ui/controls")
        {
            ADD_CTOR();
            ADD_GRID_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_GRID_HPP
