#ifndef XAML_VECTOR_HPP
#define XAML_VECTOR_HPP

#include <vector>
#include <xaml/vector.h>
#include <xaml/xaml_ptr.hpp>

XAML_META_API xaml_result xaml_vector_new(std::vector<xaml_ptr<xaml_object>>&& vec, xaml_vector** ptr) noexcept;

inline xaml_result xaml_vector_new(xaml_vector** ptr) noexcept { return xaml_vector_new({}, ptr); }

#endif // !XAML_VECTOR_HPP
