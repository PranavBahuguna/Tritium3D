#include <TritiumEngine/Rendering/TextRendering/FontLoader.hpp>
#include <TritiumEngine/Utilities/Logger.hpp>

#include <GL/glew.h>

using namespace TritiumEngine::Utilities;

namespace TritiumEngine::Rendering::TextRendering
{
  FontLoader::FontLoader(const std::string &defaultFilePath) : m_defaultFilePath(defaultFilePath) {}

  Font *FontLoader::load(const std::string &filePath) {
    FT_Library ft;
    FT_Error ftError = FT_Init_FreeType(&ft);
    if (ftError != FT_Err_Ok) {
      logFTErrorMessage(ftError);
      throw std::runtime_error("Error: FreeType library could not be initialised.");
    }

    // Try loading the font, fallback to default if unsuccessful
    Font *font = new Font();
    if (!loadFont(font, filePath, ft)) {
      Logger::warn("[FontLoader] Unable to load font from path {}, will try loading default "
                   "font instead.",
                   filePath);
      if (!loadFont(font, m_defaultFilePath, ft)) {
        Logger::error("[FontLoader] Unable to load default font.");
        return nullptr;
      }
    }

    return font;
  }

  bool FontLoader::loadFont(Font *font, const std::string &filePath, FT_Library &ft) const {
    // Try loading font face
    FT_Face face;
    FT_Error ftError = FT_New_Face(ft, filePath.c_str(), 0, &face);
    if (ftError != FT_Err_Ok) {
      logFTErrorMessage(ftError);
      return false;
    }

    // Set font pixel size
    ftError = FT_Set_Pixel_Sizes(face, 0, 64);
    if (ftError != FT_Err_Ok) {
      logFTErrorMessage(ftError);
      return false;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    for (unsigned char c = 0; c < Font::CHAR_ARRAY_SIZE; c++) {
      // Load character glyph
      ftError = FT_Load_Char(face, c, FT_LOAD_RENDER);
      if (ftError != FT_Err_Ok) {
        logFTErrorMessage(ftError);
        continue;
      }

      // Generate texture for glyph
      const auto &bitmap = face->glyph->bitmap;
      GLuint textureId;
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                   bitmap.buffer);

      // Set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Store character info in array
      font->characters[c] = {textureId,
                             {bitmap.width, bitmap.rows},
                             {face->glyph->bitmap_left, face->glyph->bitmap_top},
                             face->glyph->advance.x};
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
  }

  void FontLoader::logFTErrorMessage(const FT_Error &error) const {
    const char *errMsg;
#undef FTERRORS_H_
#define FT_ERRORDEF(e, v, s)                                                                       \
  case e:                                                                                          \
    errMsg = s;
#define FT_ERROR_START_LIST switch (error) {
#define FT_ERROR_END_LIST   }
#include FT_ERRORS_H
    errMsg = "(Unknown error)";

    Logger::error("[FontLoader] FreeType error: {}", errMsg);
  }
}; // namespace TritiumEngine::Rendering::TextRendering
