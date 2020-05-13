#ifndef XAML_UI_OBJC_H
#define XAML_UI_OBJC_H

#include <objc/objc.h>
#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif // __OBJC__

#ifndef OBJC_OBJECT
#ifdef __OBJC__
#define OBJC_OBJECT(type) type*
#else
#define OBJC_OBJECT(type) id
#endif // __OBJC__
#endif // !OBJC_OBJECT

#ifndef OBJC_NIL
#ifdef __OBJC__
#define OBJC_NIL nil
#elif defined(__cplusplus)
#define OBJC_NIL nullptr
#else
#define OBJC_NIL NULL
#endif // XAML_UI_COCOA
#endif // !OBJC_NIL

#endif // !XAML_UI_OBJC_H
