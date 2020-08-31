if(UNIX OR MINGW)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
else()
    find_path(GTK_gtk_INCLUDE_PATH gtk/gtk.h)
    find_library(GTK_gtk_LIBRARY gtk-3.0)
    find_library(GTK_gdk_LIBRARY gdk-3.0)
    find_library(GTK_pango_LIBRARY pango-1.0)
    find_library(GTK_pangocairo_LIBRARY pangocairo-1.0)
    find_library(GTK_harfbuzz_LIBRARY harfbuzz)
    find_library(GTK_atk_LIBRARY atk-1.0)
    find_library(GTK_gdk_pixbuf_LIBRARY gdk_pixbuf-2.0)

    find_package(unofficial-glib CONFIG REQUIRED)
    find_package(unofficial-cairo CONFIG REQUIRED)

    message(STATUS "Found GTK3")

    set(GTK3_FOUND TRUE)
    set(GTK3_INCLUDE_DIRS ${GTK_gtk_INCLUDE_PATH})
    set(GTK3_LIBRARIES
        ${GTK_gtk_LIBRARY} ${GTK_gdk_LIBRARY}
        ${GTK_pango_LIBRARY} ${GTK_pangocairo_LIBRARY}
        ${GTK_harfbuzz_LIBRARY} ${GTK_atk_LIBRARY} ${GTK_gdk_pixbuf_LIBRARY}
        unofficial::glib::glib unofficial::glib::gobject unofficial::glib::gio
        unofficial::cairo::cairo unofficial::cairo::cairo-gobject
    )
endif()

if(${GTK3_FOUND})
    add_library(gtk3 INTERFACE IMPORTED)
    target_link_directories(gtk3 INTERFACE ${GTK3_LIBRARY_DIRS})
    target_compile_options(gtk3 INTERFACE ${GTK3_CFLAGS_OTHER})
    target_include_directories(gtk3 INTERFACE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(gtk3 INTERFACE ${GTK3_LIBRARIES})
endif()
