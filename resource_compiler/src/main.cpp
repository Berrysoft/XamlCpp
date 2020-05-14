#include <options.h>

#ifdef UNICODE
#define _tmain wmain
#define _tcout ::std::wcout
#define _tcerr ::std::wcerr
#else
#define _tmain main
#define _tcout ::std::cout
#define _tcerr ::std::cerr
#endif // UNICODE

int _tmain(int argc, xaml_char_t** argv)
{
    return 0;
}
