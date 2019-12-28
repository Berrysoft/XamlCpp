#ifndef XAML_UI_OBJC_HPP
#define XAML_UI_OBJC_HPP

#ifdef XAML_UI_COCOA
#include <objc/objc.h>
#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif // __OBJC__
#endif // XAML_UI_COCOA

#ifndef OBJC_OBJECT
#ifdef __OBJC__
#define OBJC_OBJECT(type) type*
#else
#ifdef XAML_UI_COCOA
#define OBJC_OBJECT(type) id
#else
#define OBJC_OBJECT(type) void*
#endif // XAML_UI_COCOA
#endif // __OBJC__
#endif // !OBJC_OBJECT

#endif // !XAML_UI_OBJC_HPP
