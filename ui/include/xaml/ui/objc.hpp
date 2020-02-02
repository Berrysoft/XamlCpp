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

#ifndef OBJC_NIL
#ifdef XAML_UI_COCOA
#define OBJC_NIL nil
#else
#define OBJC_NIL nullptr
#endif // XAML_UI_COCOA
#endif // !OBJC_NIL

#ifndef OBJC_BLOCK
#if defined(__OBJC__) || !defined(XAML_UI_COCOA)
#define OBJC_BLOCK(block) block
#else
#define OBJC_BLOCK(block) ;
#endif // __OBJC__
#endif // !OBJC_BLOCK

#endif // !XAML_UI_OBJC_HPP
