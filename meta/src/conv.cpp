#include <xaml/meta/conv.hpp>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

namespace xaml
{
    std::string __wtomb(std::wstring_view str)
    {
#ifdef WIN32
        int count = WideCharToMultiByte(CP_ACP, 0, str.data(), (int)str.length(), nullptr, 0, nullptr, nullptr);
        std::string internal(count, '\0');
        WideCharToMultiByte(CP_ACP, 0, str.data(), (int)str.length(), internal.data(), (int)internal.length(), nullptr, nullptr);
        return internal;
#else
        std::size_t count = std::wcstombs(nullptr, str.data(), str.length());
        std::string internal(count, '\0');
        std::wcstombs(internal.data(), str.data(), str.length());
        return internal;
#endif // WIN32
    }

    std::wstring __mbtow(std::string_view str)
    {
#ifdef WIN32
        int count = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.data(), (int)str.length(), nullptr, 0);
        std::wstring internal(count, L'\0');
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.data(), (int)str.length(), internal.data(), (int)internal.length());
        return internal;
#else
        std::size_t count = std::mbstowcs(nullptr, str.data(), str.length());
        std::wstring internal(count, L'\0');
        std::mbstowcs(internal.data(), str.data(), str.length());
        return internal;
#endif // WIN32
    }
} // namespace xaml
