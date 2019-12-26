# XamlCpp
An implementation of XAML cross-platform framework for C++.
## Reflection
XamlCpp supports optional-reflection. All registered class could be constructed dynamically, and methods, properties and events registered could be accessed dynamically.

Reflection is heavily used by XAML dynamic parser, but it is optional if you don't want dynamic feature.

## GUI
XamlCpp is a cross-platform GUI framework. With some simple, platform-specific work, you can make your application run on all platforms supported.
### Supported platforms
|Name|API set|Supported OS|Recommanded OS|
|-|-|-|-|
|Win32|Windows API, GDI+|Windows|Windows|
|GTK+3|GLib, Cairo, Gdk, Gtk|Windows, Linux, MacOS|Linux|
|Cocoa|NeXTStep/GNUStep|Windows, Linux, MacOS|MacOS|
|WinRT*|Windows Runtime|Windows|Windows|

\* On plan

## XAML
XamlCpp uses a dialect of XAML, which may support XAML Standard in the future. Xaml files are either interpreted at run-time, or compiled to C++ code at compile-time.

The XAML interpreter and compiler(on plan) uses libxml2.
