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
    protected:
        size __get_compact_size() override { return get_size(); }

    public:
        std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

    public:
        void __draw(rectangle const& region) override;

    private:
        std::vector<grid_length> m_columns;
        std::vector<grid_length> m_rows;

    public:
        void add_column(grid_length length) { m_columns.push_back(std::move(length)); }
        void add_row(grid_length length) { m_rows.push_back(std::move(length)); }

    private:
        static std::unordered_map<std::shared_ptr<control>, grid_index> m_indecies;

    public:
        static void set_column(std::shared_ptr<control> const& c, std::size_t col) { m_indecies[c].column = col; }
        static std::size_t get_column(std::shared_ptr<control> const& c) { return m_indecies[c].column; }
        static void set_row(std::shared_ptr<control> const& c, std::size_t row) { m_indecies[c].row = row; }
        static std::size_t get_row(std::shared_ptr<control> const& c) { return m_indecies[c].row; }

#define ADD_GRID_MEMBERS() ADD_MULTICONTAINER_MEMBERS()

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_GRID_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_GRID_HPP
