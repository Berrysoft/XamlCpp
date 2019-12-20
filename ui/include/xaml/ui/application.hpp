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
        static constexpr std::string_view class_name = "application";

        friend class window;

        int wnd_num{ 0 };
        std::vector<string_t> _cmd_lines{};

#ifdef XAML_UI_GTK3
        void decrease_quit();
#endif

    public:
        application(int argc, char_t** argv)
        {
            for (int i = 0; i < argc; i++)
            {
                _cmd_lines.push_back(argv[i]);
            }
        }
        application() : application(0, nullptr) {}
#if defined(XAML_UI_WINDOWS) && defined(UNICODE)
        application(char_t* lpCmdLine);
#endif // XAML_UI_WINDOWS

        void init();
        const std::vector<string_t>& get_cmd_lines() const noexcept { return _cmd_lines; }
        int run();

        static std::shared_ptr<application> current();
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
