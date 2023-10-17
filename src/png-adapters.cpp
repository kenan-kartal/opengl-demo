#include "png-adapters.h"

#include <csetjmp>
#include <cstdio>
#include <string>

png::Image::Image(const char *const filename, png_voidp user_error_ptr,
                  png_error_ptr user_error_fn, png_error_ptr user_warning_fn) {
  using std::string_literals::operator""s;
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    throw std::runtime_error("Failed to open texture file: "s + filename);
  }
  std::vector<png_bytep> row_pointers;
  try {
    png_byte sig[8];
    if (fread(sig, 1, 8, fp) != 8) {
      throw std::runtime_error("File contains less than 8 bytes: "s + filename);
    }
    if (png_sig_cmp(sig, 0, 8) != 0) {
      throw std::runtime_error("Texture file type not supported: "s + filename);
    }
    const Read_struct read_struct{user_error_ptr, user_error_fn,
                                  user_warning_fn};
    png_structp png_ptr = read_struct.png_ptr();
    png_infop info_ptr = read_struct.info_ptr();
    if (setjmp(png_jmpbuf(png_ptr))) {
      throw std::runtime_error("PNG runtime error.");
    }
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    int bit_depth;
    int color_type;
    png_get_IHDR(png_ptr, info_ptr, &_width, &_height, &bit_depth, &color_type,
                 nullptr, nullptr, nullptr);
    png_set_expand(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
      png_set_palette_to_rgb(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0) {
      png_set_tRNS_to_alpha(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
      png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    if (bit_depth == 16) {
#if PNG_LIBPNG_VER >= 10504
      png_set_scale_16(png_ptr);
#else
      png_set_strip_16(png_ptr);
#endif
    }
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
      png_set_gray_to_rgb(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) {
      png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }
    png_read_update_info(png_ptr, info_ptr);
    // bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    // color_type = png_get_color_type(png_ptr, info_ptr);
    size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    row_pointers.resize(_height);
    _data.resize(row_size * _height);
    for (int y = 0; y < _height; y++) {
      row_pointers[y] = &_data[row_size * y];
    }
    png_read_image(png_ptr, row_pointers.data());
    png_read_end(png_ptr, nullptr);
  } catch (...) {
    fclose(fp);
    throw;
  }
  fclose(fp);
}