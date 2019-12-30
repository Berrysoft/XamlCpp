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

    template <typename TChar>
    inline constexpr TChar __star{ '*' };

    template <>
    inline constexpr wchar_t __star<wchar_t>{ L'*' };

    template <typename TChar>
    inline constexpr std::basic_string_view<TChar> __auto{ "auto" };

    template <>
    inline constexpr std::wstring_view __auto<wchar_t>{ L"auto" };

    template <typename TChar>
    inline std::vector<grid_length> __stogls(std::basic_string_view<TChar> str)
    {
        std::vector<grid_length> result;
        std::size_t offset = 0;
        do
        {
            std::size_t index = str.find_first_of(__delimeter<TChar>, offset);
            std::basic_string_view<TChar> lenstr = str.substr(offset, index - offset);
            if (lenstr == __auto<TChar>)
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
    struct __value_converter_traits<std::vector<grid_length> const&, void>
    {
        static std::vector<grid_length> convert(std::any value)
        {
            if (value.type() == typeid(std::vector<grid_length>))
            {
                return std::any_cast<std::vector<grid_length>>(value);
            }
            else if (value.type() == typeid(std::vector<grid_length> const&) || value.type() == typeid(std::vector<grid_length>&))
            {
                return std::any_cast<std::vector<grid_length> const&>(value);
            }
            else if (value.type() == typeid(std::string))
            {
                return stogls(std::any_cast<std::string>(value));
            }
            else if (value.type() == typeid(std::string_view))
            {
                return stogls(std::any_cast<std::string_view>(value));
            }
            else if (value.type() == typeid(char*) || value.type() == typeid(const char*))
            {
                return stogls(std::any_cast<const char*>(value));
            }
            else if (value.type() == typeid(std::wstring))
            {
                return stogls(std::any_cast<std::wstring>(value));
            }
            else if (value.type() == typeid(std::wstring_view))
            {
                return stogls(std::any_cast<std::wstring_view>(value));
            }
            else if (value.type() == typeid(wchar_t*) || value.type() == typeid(const wchar_t*))
            {
                return stogls(std::any_cast<const wchar_t*>(value));
            }
            else
            {
                return {};
            }
        }
        static std::any convert_back(std::vector<grid_length> const& value) { return value; }
    };

    class grid : public multicontainer, public meta_class_impl<grid>
    {
    public:
        grid();
        ~grid() override;

#ifdef XAML_UI_WINDOWS
    public:
        std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#if defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
    private:
        std::unordered_map<std::shared_ptr<control>, bool> m_put_map{};
#endif // XAML_UI_GTK3

    public:
        void __draw(rectangle const& region) override;

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
        static std::unordered_map<std::shared_ptr<control>, grid_index> m_indecies;

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
