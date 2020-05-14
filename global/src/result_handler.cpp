#include <atomic>
#include <sstream>
#include <unordered_map>
#include <xaml/event.h>
#include <xaml/result_handler.h>

using namespace std;

static atomic<int32_t> s_event_token = 0;
static unordered_map<int32_t, function<void(xaml_result, xaml_char_t const*)>> s_event_map;

void XAML_CALL xaml_result_raise(xaml_result hr, xaml_char_t const* file, int32_t line, xaml_char_t const* func) noexcept
try
{
    basic_ostringstream<xaml_char_t> stream;
    stream << file << U(':') << line << U(':') << func;
    xaml_std_string_t msg = stream.str();
    for (auto& pair : s_event_map)
    {
        pair.second(hr, msg.c_str());
    }
}
catch (...)
{
}

xaml_result XAML_CALL xaml_result_handler_add(function<void(xaml_result, xaml_char_t const*)> const& handler, int32_t* ptoken) noexcept
try
{
    int32_t token = ++s_event_token;
    s_event_map[token] = handler;
    *ptoken = token;
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

xaml_result XAML_CALL xaml_result_handler_add(void(XAML_CALL* handler)(xaml_result, xaml_char_t const*) noexcept, int32_t* ptoken) noexcept
{
    return xaml_result_handler_add(function<void(xaml_result, xaml_char_t const*)>{ handler }, ptoken);
}

xaml_result XAML_CALL xaml_result_handler_remove(int32_t token) noexcept
{
    s_event_map.erase(token);
    return XAML_S_OK;
}
