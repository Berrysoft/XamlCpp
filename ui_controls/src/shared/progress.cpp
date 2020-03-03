#include <xaml/ui/controls/progress.hpp>

using namespace std;

namespace xaml
{
    progress::progress() : control()
    {
        add_value_changed([this](progress&, size_t) { if (get_handle()) draw_progress(); });
    }

    progress::~progress() {}
} // namespace xaml
