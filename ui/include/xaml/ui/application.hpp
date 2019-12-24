#ifndef XAML_UI_APPLICATION_HPP
#define XAML_UI_APPLICATION_HPP

#include <memory>
#include <string>
#include <vector>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/strings.hpp>

namespace xaml
{
    class application : public meta_class_impl<application>, public std::enable_shared_from_this<application>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "application";

        friend class window;

        int wnd_num{ 0 };
        std::vector<string_t> m_cmd_lines{};

    private:
        application(int argc, char_t** argv)
        {
            for (int i = 0; i < argc; i++)
            {
                m_cmd_lines.push_back(argv[i]);
            }
        }
#if defined(XAML_UI_WINDOWS) && defined(UNICODE)
        application(char_t* lpCmdLine);
#endif // XAML_UI_WINDOWS

        void init_components();

    public:
        virtual ~application() {}

        const std::vector<string_t>& get_cmd_lines() const noexcept { return m_cmd_lines; }
        int run();

        static std::shared_ptr<application> init(int argc, char_t** argv);
        static std::shared_ptr<application> init() { return init(0, nullptr); }
#if defined(XAML_UI_WINDOWS) && defined(UNICODE)
        static std::shared_ptr<application> init(char_t* lpCmdLine);
#endif // XAML_UI_WINDOWS
        static std::shared_ptr<application> current();

        static void register_class() noexcept
        {
            REGISTER_TYPE();
        }
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
