#include <iostream>
#include <xaml/conv.hpp>
#include <xaml/event.hpp>

using namespace std;
using namespace xaml;

int main()
{
    primitives::string_int32_converter conv;
    cout << any_cast<int>(conv.convert(string{ "64" }, typeid(string), {}, {})) << endl;
}
