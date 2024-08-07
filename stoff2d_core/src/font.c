#include <font.h>
#include <defines.h>
#include <string.h>
#include <glad/glad.h>
#include <ft2build.h>
#include <shader.h>
#include <utils.h>
#include <rendertexture.h>
#include FT_FREETYPE_H

#define RENDER_TEX_W 1024
#define RENDER_TEX_H 1024

// Statically sized hashmap<fontname, s2dFont>, since its small just linear
// search instead of hashing for now.
s2dFont fonts[S2D_MAX_FONTS];
u32     fontCount = 0;

FT_Library ftLib;

// For render to texture.
u32 vao;
u32 vbo;
u32 shader;

s2dRenderTexture renderTexture;

void setup_gl_for_render_to_texture() { 
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    shader = s2d_shader_create(
            "engine/vRendToFont.glsl",
            "engine/fRendToFont.glsl");
    s2d_shader_use(shader);
    glUniform3f(glGetUniformLocation(shader, "textColor"), 1.0, 1.0, 1.0);
    glActiveTexture(GL_TEXTURE0);
    clmMat4 proj = clm_mat4_ortho(
        0.0f,
        RENDER_TEX_W,
        0.0f,
        RENDER_TEX_H,
        0.0f,
        1.0f);
    s2d_shader_set_uniform_mat4(
        shader,
        "projection",
        proj);
}



bool load_font(s2dFont* font, const char* fileName) {
    // create new texture to render to
    renderTexture = s2d_rendertexture_create(
            RENDER_TEX_W,
            RENDER_TEX_H,
            4,
            false);
    s2d_rendertexture_set_target(renderTexture);

    // construct path with file name and fonts folder.
    const char* fontsDir = S2D_FONTS_FOLDER;
    size_t fontPathLen = strlen(fontsDir) + strlen(fileName);
    char* fontPath = (char*) malloc((fontPathLen * sizeof(char)) + 1);
    strcpy(fontPath, fontsDir);
    strcat(fontPath, fileName);

    // remove .ttf extension and use the font name as the lookup key.
    i32 nameLength = strlen(fileName);
    char* fontName = strdup(fileName);
    fontName[nameLength - 4] = '\0'; // can't use strndup on windows!!!
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
    FT_Set_Pixel_Sizes(face, 0, 40);
    
    f32 x    = 0;
    f32 y    = 0;
    u32 rowH = 0; // the max height from all glyphs in each row.
    const u32 padding = 1;
    for (u8 c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, 
                    "[S2D Warning] could not load glyph - \'%c\' "
                    "from font - %s\n",
                    c, fontName);
            continue;
        }

        // generate glyph texture.
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
                face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // width and height of this glyph.
        float w = face->glyph->bitmap.width;
        float h = face->glyph->bitmap.rows;

        // start new row if we hit the right edge of texture.
        if (x + w > RENDER_TEX_W) {
            x = 0.0f;
            y += rowH + padding;
            rowH = 0;
        }

        // update row height. 
        if (h > rowH) {
            rowH = h;
        }

        float vertices[6][4] = {
            { x,     y + h,   0.0f, 0.0f },            
            { x,     y,       0.0f, 1.0f },
            { x + w, y,       1.0f, 1.0f },

            { x,     y + h,   0.0f, 0.0f },
            { x + w, y,       1.0f, 1.0f },
            { x + w, y + h,   1.0f, 0.0f }           
        };

        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // store the character
        font->fontTexID = renderTexture.textureID;
        font->chars[c] = (s2dChar) {
            (s2dFrame) { 
                x / (float) RENDER_TEX_W, 
                y / (float) RENDER_TEX_H, 
                w / (float) RENDER_TEX_W,
                h / (float) RENDER_TEX_H },
            face->glyph->bitmap.width, 
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left, 
            face->glyph->bitmap_top,
            face->glyph->advance.x
        };

        // now advance for next glyph
        x += w + padding; 
    }

    FT_Done_Face(face);

    glBindTexture(GL_TEXTURE_2D, renderTexture.textureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    glDeleteFramebuffers(1, &renderTexture.frameBufferID);

    return true;
}

bool font_init() {
    setup_gl_for_render_to_texture();
    memset(fonts, 0, sizeof(fonts));
    if (FT_Init_FreeType(&ftLib)) {
        fprintf(stderr, "[S2D Error] could not load freetype library\n");
        return false;
    }

    char** files    = list_files_in_dir(S2D_FONTS_FOLDER);
    char** filesCpy = files;
    char*  fileName;
    bool result = true;
    while ((fileName = *files++)) {
        // skip until the end if failed already. Need to free everything still.
        if (!result) {
            free(fileName);
            continue;
        }
        // Check if exceeded font limit.
        if (fontCount > S2D_MAX_FONTS) {
            fprintf(stderr,
                    "[S2D Error] exceeded font limit, increase this in "
                    "settings.h");
            result = false;
            free(fileName);
            continue;
        } 
        // filter out current and previous directory listing.
        if (!strcmp(".", fileName) || !strcmp("..", fileName)) {
            free(fileName);
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
            result = false;
            free(fileName);
            continue;
        }
        // Load the font.
        if (!load_font(&fonts[fontCount++], fileName)) {
            result = false;
            free(fileName);
            continue;
        }
        // reached if no errors.
        free(fileName);
    }

    free(filesCpy);
    FT_Done_FreeType(ftLib);
    // Finished rendering to textures.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return result;
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
