# XamlCpp
An implementation of XAML cross-platform framework for C++.

This project is NOT complete, and still needs a lot of work. Welcome issues and pull requests!

[![Azure DevOps builds](https://strawberry-vs.visualstudio.com/XamlCpp/_apis/build/status/Berrysoft.XamlCpp?branch=master)](https://strawberry-vs.visualstudio.com/XamlCpp/_build?definitionId=12)
## Goal
The goal of XamlCpp is to write a cross-platfrom GUI application easily and quickly. It uses a dialect of XAML to discribe the UI, but may not support all features like other XAML frameworks do. The final application should be tiny, with a few dependencies, compared to Qt or so.

## Reflection
XamlCpp supports optional-reflection. All registered class could be constructed dynamically, and methods, properties and events registered could be accessed dynamically.

A simple example is [here](./meta/test/src/main.cpp).

It doesn't use the RTTI feature of C++, instead, it ownes an stable implementation, because the constraints of the standard are too loose to implement reflection. All GUI libraries are shipped with another "meta" library, which contains functions used by the XAML parser, but is optional for the final application. Meta library is large, for it contains many instances of template classes and functions.

## GUI
XamlCpp is a cross-platform GUI framework. With some simple, platform-specific work, you can make your application run on all platforms supported.

An example is [here](./ui_controls/test/src/main.cpp)

GUI is divided into several parts.
The basic UI project is responsible for the main loop, windows and system dialogs. It is necessary to run a GUI application.

### UI
It supports desktop environment with concepts of "window" and "screen", and many dialogs like "message box" and "file box".
It also supports high DPI settings even on Windows.
#### Supported platforms
|Name|API set|Supported OS|Recommanded OS|
|-|-|-|-|
|Win32|Windows API, GDI+|Windows\*|Windows|
|GTK+3|GLib, Cairo, Gdk, Gtk|Windows, Linux, MacOS|Linux, Windows/MinGW|
|Cocoa|Cocoa|MacOS|MacOS|

\* At least Windows 7.

### Controls
Common controls, works on all platforms.

### Canvas
A canvas control.
#### Supported platforms
|Name|API set|
|-|-|
|Win32|Direct2D -> GDI+ \*|
|GTK+3|Cairo|
|Cocoa|NSGraphicsContext|

\* Indicates the search order. GDI is not supported because it lacks some key features.

### WebView
A webview control.
#### Supported platforms
|Name|API set|
|-|-|
|Win32|Edge(Chromium) -> Edge -> IE \*|
|GTK+3|Webkit \*\*|
|Cocoa|WebKit|

\* Indicates the search order. IE is supported to ensure it could be used.

\*\* Linux only because of the limitation of webkit2gtk.

## XAML
XamlCpp uses a dialect of XAML, which may support XAML Standard in the future. XAML files are either interpreted at run-time, or compiled to C++ code at compile-time.

Here's an XAML example:
``` xml
<window x:class="xaml::test::test_window"
        xmlns="https://github.com/Berrysoft/XamlCpp/"
        xmlns:x="https://github.com/Berrysoft/XamlCpp/xaml/"
        title="Test Window" location="200, 100" size="800, 600">
  <grid margin="10" columns="1*, 0.8*, 1*" rows="1*, auto, 1*">
    <label x:name="mylabel" grid.column="0" grid.row="1" margin="5, 0" text_halignment="right" valignment="center" text="{binding text,element=mybutton,mode=one_way}"/>
    <button x:name="mybutton" grid.column="1" grid.row="1" click="on_button_click">Hello</button>
    <canvas grid.column="0" grid.row="2" margin="10" redraw="on_canvas_redraw"/>
  </grid>
</window>
```
The `xmlns` should be `https://github.com/Berrysoft/XamlCpp/` to use default controls. The xml attributes decribes the properties, events and attach properties of a control.

The XAML parser uses a modified version of [rapidxml_ns](https://github.com/svgpp/rapidxml_ns). It is modified to use features of C++ 17 and simplified to support UTF-8 only for convenience.

## XAML Compiler
The reflection component is a little heavy and slow, compared to static-compiled code. XAML Compiler solves the problem by compiling XAML to native C++ code. The XAML code above may be compiled to:

``` c++
void ::xaml::test::test_window::init_components()
{
    this->set_title(U("Test Window"));
    this->set_location({ 200, 100 });
    this->set_size({ 800, 600 });
    auto __N4xaml4gridE__1 = ::std::make_shared<::xaml::grid>();
    __N4xaml4gridE__1->set_margin({ 10, 10, 10, 10 });
    __N4xaml4gridE__1->set_columns({ { 1, ::xaml::grid_layout::star }, { 0.8, ::xaml::grid_layout::star }, { 1, ::xaml::grid_layout::star } });
    __N4xaml4gridE__1->set_rows({ { 1, ::xaml::grid_layout::star }, { 0, ::xaml::grid_layout::compact }, { 1, ::xaml::grid_layout::star } });
    auto mylabel = ::std::make_shared<::xaml::label>();
    ::xaml::grid::set_column(mylabel, 0);
    ::xaml::grid::set_row(mylabel, 1);
    mylabel->set_margin({ 5, 0, 5, 0 });
    mylabel->set_text_halignment(::xaml::halignment::right);
    mylabel->set_valignment(::xaml::valignment::center);
    __N4xaml4gridE__1->add_child(mylabel);
    auto mybutton = ::std::make_shared<::xaml::button>();
    ::xaml::grid::set_column(mybutton, 1);
    ::xaml::grid::set_row(mybutton, 1);
    mybutton->set_text(U("Hello"));
    mybutton->add_click(::xaml::mem_fn_bind(on_button_click, this));
    __N4xaml4gridE__1->add_child(mybutton);
    auto __N4xaml6canvasE__3 = ::std::make_shared<::xaml::canvas>();
    ::xaml::grid::set_column(__N4xaml6canvasE__3, 0);
    ::xaml::grid::set_row(__N4xaml6canvasE__3, 2);
    __N4xaml6canvasE__3->set_margin({ 10, 10, 10, 10 });
    __N4xaml6canvasE__3->add_redraw(::xaml::mem_fn_bind(on_canvas_redraw, this));
    __N4xaml4gridE__1->add_child(__N4xaml6canvasE__3);
    this->set_child(__N4xaml4gridE__1);
    ::xaml::bind(mylabel, "text", mybutton, "text", ::xaml::binding_mode::one_way);
}
```

## Build
A C++17-compliant compiler is required.

This project assumes it is built by GCC 9.1+ or Clang 9.0+ when using GNU or LLVM toolchains. If building with previous versions, you should manully link it with `libstdc++fs` or `libc++fs`.
### Build on Windows
#### MSVC
`wil` is required. `vcpkg` is recommanded. Other packages will be downloaded from NuGet when configuring.
#### MinGW
`gtk` and `pkgconfig` are required. MSYS2 is recommanded for installing and building.

It is not possible now to build target Windows API with MinGW toolchain, because `wil` isn't supported by GCC and Clang/MinGW.
### Build on Linux
`gtk`, `webkit2gtk` and `pkgconfig` are required.
### Build on Mac
Mac OS 10.14+ is required for some C++17 features.
#### Cocoa
No other packages needed.
#### Gtk
`gtk` and `pkgconfig` are also required.
