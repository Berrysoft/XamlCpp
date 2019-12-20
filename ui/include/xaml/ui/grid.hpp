#ifndef XAML_UI_GRID_HPP
#define XAML_UI_GRID_HPP

#include <unordered_map>
#include <vector>
#include <xaml/ui/container.hpp>

namespace xaml
{
    enum class grid_layout
    {
        abs,
        star
    };

    struct grid_length
    {
        double value;
        grid_layout layout;
    };

    struct grid_index
    {
        size_t column, row;
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
        LRESULT wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

    public:
        void draw(rectangle const& region) override;

#ifdef XAML_UI_WINDOWS
    public:
        using native_handle_type = typename control::native_handle_type;

        native_handle_type get_handle() const noexcept override;
#endif // XAML_UI_WINDOWS

    private:
        std::vector<grid_length> m_columns;
        std::vector<grid_length> m_rows;

    public:
        void add_column(grid_length length) { m_columns.push_back(std::move(length)); }
        void add_row(grid_length length) { m_rows.push_back(std::move(length)); }

    private:
        std::unordered_map<std::shared_ptr<control>, grid_index> m_indecies;

    public:
        void set_column(std::shared_ptr<control> const& c, size_t col) { m_indecies[c].column = col; }
        void set_row(std::shared_ptr<control> const& c, size_t row) { m_indecies[c].row = row; }

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
