#ifndef XAML_VECTOR_HPP
#define XAML_VECTOR_HPP

#include <vector>
#include <xaml/vector.h>
#include <xaml/xaml_ptr.hpp>

XAML_META_API xaml_ptr<xaml_vector> xaml_make_vector(std::vector<xaml_ptr<xaml_object>>&& vec = {});

#endif // !XAML_VECTOR_HPP
