#ifndef PNG_ADAPTERS_H
#define PNG_ADAPTERS_H

#include <png.h>

#include <stdexcept>
#include <vector>

namespace png {

class Read_struct {
public:
  explicit Read_struct(png_voidp user_error_ptr, png_error_ptr user_error_fn,
                       png_error_ptr user_warning_fn) {
    _png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, user_error_ptr,
                                      user_error_fn, user_warning_fn);
    if (!_png_ptr) {
      throw std::runtime_error("Failed to create PNG read struct.");
    }
    _info_ptr = png_create_info_struct(_png_ptr);
    if (!_info_ptr) {
      png_destroy_read_struct(&_png_ptr, nullptr, nullptr);
      throw std::runtime_error("Failed to create PNG info struct.");
    }
  }
  ~Read_struct() { png_destroy_read_struct(&_png_ptr, &_info_ptr, nullptr); }

  png_structp png_ptr() const { return _png_ptr; }
  png_infop info_ptr() const { return _info_ptr; }

private:
  png_structp _png_ptr{};
  png_infop _info_ptr{};
};

class Image {
public:
  explicit Image(const char *const filename, png_voidp user_error_ptr,
                 png_error_ptr user_error_fn, png_error_ptr user_warning_fn);

  png_uint_32 width() const { return _width; }
  png_uint_32 height() const { return _height; }
  const std::vector<png_byte> &vector() const { return _data; }

private:
  png_uint_32 _width;
  png_uint_32 _height;
  std::vector<png_byte> _data;
};

} // namespace png

#endif
