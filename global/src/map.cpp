#include <xaml/map.h>

using namespace std;

xaml_result XAML_CALL xaml_hasher_string_default(xaml_hasher<xaml_string>** ptr) noexcept
{
    return xaml_hasher_new<xaml_string>(
        fu2::unique_function<xaml_result(xaml_string*, std::int32_t*) noexcept>{
            [](xaml_string* value, int32_t* phash) noexcept -> xaml_result {
                static hash<std::string_view> hasher{};
                string_view data;
                XAML_RETURN_IF_FAILED(to_string_view(value, &data));
                std::size_t const std_hash = hasher(data);
                std::int32_t const* ptr = reinterpret_cast<std::int32_t const*>(&std_hash);
#if SIZE_MAX == UINT64_MAX
                static_assert(sizeof(XAML_STD size_t) == sizeof(XAML_STD uint64_t), "Unknown 64-bit platform.");
                *phash = ptr[0] ^ ptr[1];
#elif SIZE_MAX == UINT32_MAX
                static_assert(sizeof(XAML_STD size_t) == sizeof(XAML_STD uint32_t), "Unknown 32-bit platform.");
                *phash = ptr[0];
#else
    #error Cannot determine platform architecture
#endif
                return XAML_S_OK;
            } },
        fu2::unique_function<xaml_result(xaml_string*, xaml_string*, bool*) noexcept>{
            [](xaml_string* lvalue, xaml_string* rvalue, bool* pb) noexcept -> xaml_result {
                return xaml_string_equals(lvalue, rvalue, pb);
            } },
        ptr);
}
