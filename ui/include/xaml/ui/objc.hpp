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

#ifndef OBJC_INTERNAL_OBJECT
#ifdef __OBJC__
#ifdef XAML_UI_COCOA_INTERNAL
#define OBJC_INTERNAL_OBJECT(type) type*
#else
#define OBJC_INTERNAL_OBJECT(type) id
#endif // XAML_UI_COCOA_INTERNAL
#else
#ifdef XAML_UI_COCOA
#define OBJC_INTERNAL_OBJECT(type) id
#else
#define OBJC_INTERNAL_OBJECT(type) void*
#endif // XAML_UI_COCOA
#endif // __OBJC__
#endif // !OBJC_INTERNAL_OBJECT

#endif // !XAML_UI_OBJC_HPP
