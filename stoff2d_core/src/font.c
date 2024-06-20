#include <font.h>
#include <defines.h>
#include <dirent.h> 
#include <string.h>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

// Statically sized hashmap<fontname, s2dFont>, since its small just linear
// search instead of hashing for now.
s2dFont fonts[S2D_MAX_FONTS];
u32     fontCount = 0;

FT_Library ftLib;

bool load_font(s2dFont* font, const char* fileName) {
    // construct path with file name and fonts folder.
    const char* fontsDir = S2D_FONTS_FOLDER;
    size_t fontPathLen = strlen(fontsDir) + strlen(fileName);
    char* fontPath = (char*) malloc((fontPathLen * sizeof(char)) + 1);
    strcpy(fontPath, fontsDir);
    strcat(fontPath, fileName);

    // remove .ttf extension and use the font name as the lookup key.
    i32 nameLength = strlen(fileName);
    char* fontName = strndup(fileName, nameLength - 4); 
    font->fontName = fontName; // INFO: fontName freed in shutdown

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // load face using freetype.
    FT_Face face;
    if (FT_New_Face(ftLib, fontPath, 0, &face)) {
        fprintf(stderr,
                "[S2D Error] could not load font - %s\n",
                fontPath);
        return false;
    }
    free(fontPath);
    FT_Set_Pixel_Sizes(face, 0, 16); // TODO: make this configurable
    for (u8 c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, 
                    "[S2D Warning] could not load glyph - \'%c\' "
                    "from font - %s\n",
                    c, fontName);
            continue;
        }
        // generate texture
        u32 texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // store the character
        font->chars[c] = (s2dChar) {
            texture, 
            face->glyph->bitmap.width, 
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left, 
            face->glyph->bitmap_top,
            face->glyph->advance.x
        };
    }

    FT_Done_Face(face);

    return true;
}

bool font_init() {
    // Initialise freetype.
    if (FT_Init_FreeType(&ftLib)) {
        fprintf(stderr, "[S2D Error] could not load freetype library\n");
        return false;
    }

    // clear out fonts.
    memset(fonts, 0, sizeof(fonts));

    // load all ttf fonts in S2D_FONTS_FOLDER
    struct dirent* dirEntry;
    DIR* dir;
    if (!(dir = opendir(S2D_FONTS_FOLDER))) { 
        fprintf(stderr, 
                "[S2D Error] could not open fonts folder - %s\n", 
                S2D_FONTS_FOLDER); 
        return false; 
    } 
    while ((dirEntry = readdir(dir))) {
        // Check if exceeded font limit.
        if (fontCount > S2D_MAX_FONTS) {
            fprintf(stderr,
                    "[S2D Error] exceeded font limit, increase this in "
                    "settings.h");
            return false;
        }

        const char* fileName = dirEntry->d_name;
        if (!strcmp(".", fileName) || !strcmp("..", fileName)) {
            continue;
        }
        // Check if each file name is a valid ttf file before processing.
        i32 nameLength = strlen(fileName);
        i32 end = nameLength - 1;
        if (nameLength < 5 || 
                fileName[end - 3] != '.' || 
                fileName[end - 2] != 't' || 
                fileName[end - 1] != 't' ||
                fileName[end - 0] != 'f') {
            fprintf(stderr, 
                    "[S2D Error] font file %s must have a name and be a ttf "
                    "font\n",
                    fileName);
            return false;
        }
        // Load the font.
        if (!load_font(&fonts[fontCount++], fileName)) {
            return false;
        }
    }

    closedir(dir);     
    FT_Done_FreeType(ftLib);

    return true;
}

void font_shutdown() {
    for (u32 i = 0; i < fontCount; i++) {
        free(fonts[i].fontName);
    }
}

s2dFont* font_get_font(const char* fontName) {
    s2dFont* font = NULL;
    for (u32 i = 0; i < fontCount; i++) {
        if (!strcmp(fonts[i].fontName, fontName)) {
            font = &fonts[i];
            break;
        }
    }
    return font;
}
