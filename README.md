# XamlCpp
An implementation of XAML cross-platform framework for C++.

This project is not complete, and still needs a lot of work. Welcome issues and pull requests!

[![Azure DevOps builds](https://strawberry-vs.visualstudio.com/XamlCpp/_apis/build/status/Berrysoft.XamlCpp?branch=master)](https://strawberry-vs.visualstudio.com/XamlCpp/_build?definitionId=12)
## Reflection
XamlCpp supports optional-reflection. All registered class could be constructed dynamically, and methods, properties and events registered could be accessed dynamically.

A simple example is [here](./meta/test/main.cpp).

Reflection is heavily used by XAML parser, but it is optional if you don't want dynamic feature.

## GUI
XamlCpp is a cross-platform GUI framework. With some simple, platform-specific work, you can make your application run on all platforms supported.

An example is [here](./ui/test/main.cpp)

### Supported platforms
|Name|API set|Supported OS|Recommanded OS|
|-|-|-|-|
|Win32|Windows API, GDI+|Windows|Windows|
|GTK+3|GLib, Cairo, Gdk, Gtk|Windows, Linux, MacOS|Linux|
|Cocoa|NeXTStep|MacOS|MacOS|
|WinRT|Windows Runtime|Windows|Windows|

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

The XAML parser uses libxml2 on non-WinRT platforms. For Windows developers, you can install it with `vcpkg`.

## XAML Compiler
The reflection component is a little heavy and slow, compared to static-compiled code. XAML Compiler solves the problem by compiling XAML to native C++ code. The XAML code above may be compiled to:

``` c++
namespace xaml::test
{
    void test_window::init_components()
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
}
```
