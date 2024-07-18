/*!
 * Canvas class implementation.
 * @file canvas.cpp
 */

#include "canvas.h"
#include "lodepng.h"
#include <fstream>
#include <cstdio>   
#include <cstdlib>
#include <stdexcept>
#include <string>


namespace life {

  // TODO: Adicione o resto da implementação dos métodos aqui.
  //=== Special members
  /// Constructor
  /*! Creates an empty canvas with the requested dimensions.
   * @param w The canvas width in virtual pixels.
   * @param h The canvas height in virtual pixels.
   * @param bs The canvas block size in real pixels.
   */
  Canvas :: Canvas(size_t w, size_t h, short bs)
    :   m_width(w), m_height(h),m_block_size(bs), m_pixels(w * h * image_depth, 0) {clear(BLACK);}
  
  /*!
   * Deep copy of the canvas.
   * @param clone The object we are copying from.
   */
  Canvas :: Canvas(const Canvas& clone)
    : m_width(clone.m_width), m_height(clone.m_height), m_block_size(clone.m_block_size), m_pixels(clone.m_pixels) {}
  
  /*!
   * @param source The object we are copying information from.
   * @return A reference to the `this` object.
   */
  Canvas& Canvas::operator=(const Canvas& source) { 
    if (this != &source) {
      m_width = source.m_width;
      m_height = source.m_height;
      m_block_size = source.m_block_size;
      m_pixels = source.m_pixels;
    }
    return *this;
  }

  //=== Members
  /// Assigns a black color to the whole image.
  void Canvas :: clear(const Color& color) {
    for (size_t i = 0; i < m_pixels.size(); i += image_depth) {
      m_pixels[i] = color.channels[Color::R];
      m_pixels[i + 1] = color.channels[Color::G];
      m_pixels[i + 2] = color.channels[Color::B];
      m_pixels[i + 3] = 255;  // Alpha channel
    }
  }
  
  /*!
   * Draw a pixel on the virtual image at the requested coordinate.
   *
   * @note Nothing is done if the  pixel coordinate is located outside the canvas.
   * @param x The (virtual) X coordinate of the pixel we want to know the color of.
   * @param y The (virtual) Y coordinate of the pixel we want to know the color of.
   * @param c The color.
   */
  void Canvas :: pixel(coord_t x, coord_t y, const Color& color) {
  if (x < m_width && y < m_height) {
    size_t index = (y * m_width + x) * image_depth;
    m_pixels[index] = color.channels[Color::R];
    m_pixels[index + 1] = color.channels[Color::G];
    m_pixels[index + 2] = color.channels[Color::B];
    m_pixels[index + 3] = 255;  // Alpha channel
    }
  }

  /*!
   * @throw `std::invalid_argument()` it the pixel coordinate is located outside the canvas.
   * @param x The (virtual) X coordinate of the pixel we want to know the color of.
   * @param y The (virtual) Y coordinate of the pixel we want to know the color of.
   * @return The pixel color.
   */
  Color Canvas :: pixel(coord_t x, coord_t y) const {
  if (x < m_width && y < m_height) {
    size_t index = (y * m_width + x) * image_depth;
    return Color(m_pixels[index], m_pixels[index + 1], m_pixels[index + 2]);
  }
  return BLACK;  
  }

  /// Save canvas as an PPM.
  bool Canvas :: save(const unsigned char* data, size_t w, size_t h, size_t d, const std :: string& filename) {
    std :: ofstream file(filename, std :: ios :: out);
    if (!file.is_open()){
      throw std :: runtime_error("Unable to open the file.");
    }
    file << "P3\n" << w << " " << h << "\n255\n";
    for (size_t i = 0; i < w * h * d; i += d) {
      file << (int)data[i] << " "
           << (int)data[i + 1] << " "
           << (int)data[i + 2] << "\n";
    }
    file.close();
    return true;
  }

  /// Save canvas as an png
  void Canvas :: encode_png(std :: string&  filename, const unsigned char* image, unsigned width, unsigned height){
    unsigned error = lodepng::encode(filename, image, width, height);
    if (error != 0U) {
      std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
  }

  /// Starts a canvas object.
  void Canvas :: start_canva(short size, size_t width, size_t height){
    m_block_size = size;
    m_height = height;
    m_width = width;
  }

}  // namespace life

//================================[ canvas.cpp ]================================//
