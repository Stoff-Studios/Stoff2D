#include <font.h>
#include <defines.h>
#include <dirent.h> 
#include <string.h>
#include <glad/glad.h>
#include <ft2build.h>
#include <shader.h>
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

void setup_gl_for_render_to_texture() { 
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    shader = shader_create(
            "vRendToFont.glsl",
            "fRendToFont.glsl");
    shader_use(shader);
    glUniform3f(glGetUniformLocation(shader, "textColor"), 1.0, 1.0, 1.0);
    glActiveTexture(GL_TEXTURE0);
    clmMat4 proj = clm_mat4_ortho(
        0.0f,
        RENDER_TEX_W,
        0.0f,
        RENDER_TEX_H,
        0.0f,
        1.0f);
    shader_set_uniform_mat4(
        shader,
        "projection",
        proj);
}

// returns the texture that will be rendered to.
u32 new_rendertexture_framebuffer() {
    // framebuffer.
    u32 frameBuffer = 0;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

    // render texture.
    u32 renderedTexture;
    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            RENDER_TEX_W,
            RENDER_TEX_H,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glFramebufferTexture(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            renderedTexture,
            0);
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "[S2D Error] failed to create font render texture\n");
    }

    glViewport(0, 0, RENDER_TEX_W, RENDER_TEX_H);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    return renderedTexture;
}

bool load_font(s2dFont* font, const char* fileName) {
    // create new framebuffer to render to
    u32 renderTexture = new_rendertexture_framebuffer();

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
    FT_Set_Pixel_Sizes(face, 0, 40); // TODO: make this configurable
    
    f32 x    = 0;
    f32 y    = 0;
    u32 rowH = 0; // the max height from all glyphs in each row.
    u32 padding = 2;
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
                face->glyph->bitmap.buffer
                );
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
        font->fontTexID = renderTexture;
        font->chars[c] = (s2dChar) {
            (Frame) { 
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
        x += w; 
    }

    FT_Done_Face(face);

    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

bool font_init() {
    // Initialise freetype.
    if (FT_Init_FreeType(&ftLib)) {
        fprintf(stderr, "[S2D Error] could not load freetype library\n");
        return false;
    }

    // Init opengl
    setup_gl_for_render_to_texture();

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

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

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
