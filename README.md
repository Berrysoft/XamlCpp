# XamlCpp
An implementation of XAML cross-platform framework for C++.

This project is not complete, and still needs a lot of work. Welcome issues and pull requests!
## Reflection
XamlCpp supports optional-reflection. All registered class could be constructed dynamically, and methods, properties and events registered could be accessed dynamically.

A simple example is [here](./meta/test/main.cpp).

Reflection is heavily used by XAML dynamic parser, but it is optional if you don't want dynamic feature.

## GUI
XamlCpp is a cross-platform GUI framework. With some simple, platform-specific work, you can make your application run on all platforms supported.

An example is [here](./ui/test/main.cpp)

### Supported platforms
|Name|API set|Supported OS|Recommanded OS|
|-|-|-|-|
|Win32|Windows API, GDI+|Windows|Windows|
|GTK+3|GLib, Cairo, Gdk, Gtk|Windows, Linux, MacOS|Linux|
|Cocoa|NeXTStep|MacOS|MacOS|
|WinRT*|Windows Runtime|Windows|Windows|

\* On plan

## XAML
XamlCpp uses a dialect of XAML, which may support XAML Standard in the future. XAML files are either interpreted at run-time, or compiled to C++ code at compile-time.

Here's an XAML example:
``` xml
<window xmlns="https://github.com/Berrysoft/XamlCpp/"
        title="Test Window" location="200, 100" size="800, 600">
  <grid margin="10" columns="1*, 0.8*, 1*" rows="1*, auto, 1*">
    <label grid.column="0" grid.row="1" text_halignment="right" valignment="center" text="This is a label."/>
    <button grid.column="1" grid.row="1" text="Hello" click="on_button_click"/>
  </grid>
</window>
```
The `xmlns` should be `https://github.com/Berrysoft/XamlCpp/` to use default controls. The xml attributes decribes the properties, events and attach properties of a control.

The XAML interpreter and compiler(on plan) uses libxml2. For Windows developers, you can install it with `vcpkg`.
