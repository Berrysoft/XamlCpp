#ifndef XAML_UI_GRID_HPP
#define XAML_UI_GRID_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <xaml/ui/container.hpp>

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

    class grid : public multicontainer, public meta_class_impl<grid>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "grid";

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

#define ADD_GRID_MEMBERS()        \
    ADD_MULTICONTAINER_MEMBERS(); \
    ADD_PROP(columns);            \
    ADD_PROP(rows)

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_GRID_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_GRID_HPP
