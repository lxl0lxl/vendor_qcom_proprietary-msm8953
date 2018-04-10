// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
// Copyright 2013-2015 Qualcomm Technologies, Inc.
// All rights reserved.
// Confidential and Proprietary – Qualcomm Technologies, Inc.
// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
#pragma once

#include <cstddef>
#include <string>

#include <mare/bufferiterator.hh>
#include <mare/devicetypes.hh>
#include <mare/memregion.hh>
#include <mare/texturetype.hh>

#include <mare/internal/buffer/buffer.hh>
#include <mare/internal/compat/compilercompat.h>
#include <mare/internal/util/debug.hh>
#include <mare/internal/util/strprintf.hh>

namespace mare {

template<typename T>
buffer_ptr<T> create_buffer(size_t num_elems)
{
  return create_buffer<T>(num_elems, true, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(size_t num_elems,
                            bool conservative_host_sync)
{
  return create_buffer<T>(num_elems, conservative_host_sync, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(size_t num_elems,
                            device_set const& likely_devices)
{
  return create_buffer<T>(num_elems, true, likely_devices);
}

template<typename T>
buffer_ptr<T> create_buffer(size_t num_elems,
                            bool conservative_host_sync,
                            device_set const& likely_devices);

template<typename T>
buffer_ptr<T> create_buffer(T* preallocated_ptr,
                            size_t num_elems)
{
  return create_buffer<T>(preallocated_ptr, num_elems, true, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(T* preallocated_ptr,
                            size_t num_elems,
                            bool conservative_host_sync)
{
  return create_buffer<T>(preallocated_ptr, num_elems, conservative_host_sync, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(T* preallocated_ptr,
                            size_t num_elems,
                            device_set const& likely_devices)
{
  return create_buffer<T>(preallocated_ptr, num_elems, true, likely_devices);
}

template<typename T>
buffer_ptr<T> create_buffer(T* preallocated_ptr,
                            size_t num_elems,
                            bool conservative_host_sync,
                            device_set const& likely_devices);

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr)
{
  return create_buffer<T>(mr, 0, true, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr,
                            size_t num_elems)
{
  return create_buffer<T>(mr, num_elems, true, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr,
                            bool conservative_host_sync)
{
  return create_buffer<T>(mr, 0, conservative_host_sync, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr,
                            device_set const& likely_devices)
{
  return create_buffer<T>(mr, 0, true, likely_devices);
}

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr,
                            size_t num_elems,
                            bool conservative_host_sync)
{
  return create_buffer<T>(mr, num_elems, conservative_host_sync, device_set());
}

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr,
                            size_t num_elems,
                            device_set const& likely_devices)
{
  return create_buffer<T>(mr, num_elems, true, likely_devices);
}

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr,
                            bool conservative_host_sync,
                            device_set const& likely_devices)
{
  return create_buffer<T>(mr, 0, conservative_host_sync, likely_devices);
}

template<typename T>
buffer_ptr<T> create_buffer(memregion const& mr,
                            size_t num_elems,
                            bool conservative_host_sync,
                            device_set const& likely_devices);

MARE_GCC_IGNORE_BEGIN("-Weffc++");

template<typename T>
class buffer_ptr : private internal::buffer_ptr_base {

public:

  typedef buffer_iterator<T> iterator;

  typedef buffer_const_iterator<T> const_iterator;

  buffer_ptr() :
    base(),
    _num_elems(0)
  {}

  buffer_ptr(buffer_ptr< typename std::remove_const<T>::type > const& other) :
    base(reinterpret_cast<buffer_ptr_base const&>(other)),
    _num_elems(other.size())
  {}

  buffer_ptr& operator=(buffer_ptr< typename std::remove_const<T>::type > const& other)
  {
    if(static_cast<void const*>(this) == &other)
      return *this;

    base::operator=(reinterpret_cast<buffer_ptr_base const&>(other));
    _num_elems = other.size();
    return *this;
  }

  bool syncs_on_task_finish() const { return base::syncs_on_task_finish(); }

  bool is_exclusive_to_task() const { return base::is_exclusive_to_task(); }

  void sync() {
    base::sync();
  }

  inline
  void* get_host_accessible_data() const {
    return base::get_host_accessible_data();
  }

#ifdef MARE_HAVE_OPENCL
  template<mare::graphics::image_format img_format, int dims>
  buffer_ptr& treat_as_texture(mare::graphics::image_size<dims> const& is) {
    base::treat_as_texture<img_format, dims>(is);
    return *this;
  }
#endif

  inline
  size_t size() const { return _num_elems; }

  inline
  T& operator[](size_t index) const {
    return reinterpret_cast<T*>(get_host_accessible_data())[index];
  }

  inline
  T& at(size_t index) const {

    MARE_API_THROW(get_host_accessible_data() != nullptr,
                   "buffer data is currently not host accessible");

    MARE_API_THROW_CUSTOM(0 <= index && index < _num_elems,
                          std::out_of_range,
                          "Out of bounds: index=%zu num_elems=%zu",
                          index,
                          _num_elems);

    return operator[](index);
  }

  iterator begin() const { return iterator(const_cast<buffer_ptr<T>*>(this), 0); }

  iterator end()   const { return iterator(const_cast<buffer_ptr<T>*>(this), size()); }

  const_iterator cbegin() const { return const_iterator(this, 0); }

  const_iterator cend()   const { return const_iterator(this, size()); }

  std::string to_string() const {
    std::string s = ::mare::internal::strprintf("num_elems=%zu sizeof(T)=%zu ",
                                                _num_elems,
                                                sizeof(T));
    s += base::to_string();
    return s;
  }

private:

  typedef internal::buffer_ptr_base base;

  typedef typename std::remove_cv<T>::type bare_T;

  size_t _num_elems;

  buffer_ptr(size_t num_elems,
             bool conservative_host_sync,
             device_set const& device_hints) :
    base(num_elems * sizeof(T),
         conservative_host_sync,
         device_hints),
    _num_elems(num_elems)
  {}

  buffer_ptr(T* preallocated_ptr,
             size_t num_elems,
             bool conservative_host_sync,
             device_set const& device_hints) :
    base(const_cast<bare_T*>(preallocated_ptr),
         num_elems * sizeof(T),
         conservative_host_sync,
         device_hints),
    _num_elems(num_elems)
  {}

  buffer_ptr(memregion const& mr,
             size_t num_elems,
             bool conservative_host_sync,
             device_set const& device_hints) :
    base(mr,
         num_elems > 0 ? num_elems * sizeof(T) : mr.get_num_bytes(),
         conservative_host_sync,
         device_hints),
    _num_elems(num_elems > 0 ? num_elems : mr.get_num_bytes() / sizeof(T))
  {
    MARE_API_ASSERT(num_elems == 0 || num_elems * sizeof(T) <= mr.get_num_bytes(),
                    "num_elems=%zu exceed the capacity of the memory region size=%zu bytes",
                    num_elems, mr.get_num_bytes());
  }

  friend
  buffer_ptr<T> create_buffer<T>(size_t num_elems,
                                 bool conservative_host_sync,
                                 device_set const& likely_devices);

  friend
  buffer_ptr<T> create_buffer<T>(T* preallocated_ptr,
                                 size_t num_elems,
                                 bool conservative_host_sync,
                                 device_set const& likely_devices);

  friend
  buffer_ptr<T> create_buffer<T>(memregion const& mr,
                                 size_t num_elems,
                                 bool conservative_host_sync,
                                 device_set const& likely_devices);

  inline
  bool is_null() const { return base::is_null(); }

  inline
  void const* get_buffer() const { return base::get_buffer(); }

  template<typename U>
  friend
  bool operator==(::mare::buffer_ptr<U> const& b, ::std::nullptr_t);

  template<typename U>
  friend
  bool operator==(::std::nullptr_t, ::mare::buffer_ptr<U> const& b);

  template<typename U>
  friend
  bool operator==(::mare::buffer_ptr<U> const& b1, ::mare::buffer_ptr<U> const& b2);

};

MARE_GCC_IGNORE_END("-Weffc++");

template<typename T>
bool operator==(::mare::buffer_ptr<T> const& b, ::std::nullptr_t) { return b.is_null(); }

template<typename T>
bool operator==(::std::nullptr_t, ::mare::buffer_ptr<T> const& b) { return b.is_null(); }

template<typename T>
bool operator==(::mare::buffer_ptr<T> const& b1, ::mare::buffer_ptr<T> const& b2)
{
  return b1.get_buffer() == b2.get_buffer();
}

template<typename T>
bool operator!=(::mare::buffer_ptr<T> const& b, ::std::nullptr_t) { return !(b == nullptr); }

template<typename T>
bool operator!=(::std::nullptr_t, ::mare::buffer_ptr<T> const& b) { return !(nullptr == b); }

template<typename T>
bool operator!=(::mare::buffer_ptr<T> const& b1, ::mare::buffer_ptr<T> const& b2)
{
  return !(b1 == b2);
}

namespace mem_access {

template<typename T>
buffer_ptr<T> exclusive_nosync(buffer_ptr<T> const& b);

template<typename T>
buffer_ptr<T> exclusive_sync(buffer_ptr<T> const& b);

template<typename T>
buffer_ptr<T> shared_nosync(buffer_ptr<T> const& b);

template<typename T>
buffer_ptr<T> shared_sync(buffer_ptr<T> const& b);

};

template<typename BufferPtr>
struct in;

template<typename BufferPtr>
struct out;

template<typename BufferPtr>
struct inout;

};
