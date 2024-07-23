
#include <quad_renderer.h>

#include <glad/glad.h>
#include <shader.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    // opengl buffer objects
    u32 vao;
    u32 vbo;
    u32 ebo;

    // vertex buffer
    s2dVertex  vertices[S2D_MAX_VERTICES];
    s2dVertex* currentVertex;

    // shader and texture used on last draw call
    u32 lastShader;
    u32 lastTexture;

    // batch counts
    u32 indicesCount;
    u32 verticesCount;

    // stats
    u32 drawCalls;

} _QuadRenderer;


// sets up opengl buffers and batch rendering related paraphernalia 
void init_buffers(_QuadRenderer* renderer) {
    // gen vao
    glGenVertexArrays(1, &renderer->vao); 
    glBindVertexArray(renderer->vao);

    // gen vbo
    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            S2D_MAX_VERTICES * sizeof(s2dVertex),
            NULL,
            GL_DYNAMIC_DRAW);

    // attribute 0, position
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(
            0,                                      // attribute no.
            2,                                      // number of elements
            GL_FLOAT,                               // data type of the elements
            GL_FALSE,                               // normalise
            sizeof(s2dVertex),                      // stride
            (void*) offsetof(s2dVertex, position)); // offset

    // attribute 1, texCoords
    glEnableVertexAttribArray(1);  
    glVertexAttribPointer(
            1,                                      // attribute no.
            2,                                      // number of elements
            GL_FLOAT,                               // data type of the elements
            GL_FALSE,                               // normalise
            sizeof(s2dVertex),                      // stride
            (void*) offsetof(s2dVertex, texCoord)); // offset
    
    // attribute 2, colour
    glEnableVertexAttribArray(2);  
    glVertexAttribPointer(
            2,                                    // attribute no.
            4,                                    // number of elements
            GL_FLOAT,                             // data type of the elements
            GL_FALSE,                             // normalise
            sizeof(s2dVertex),                    // stride
            (void*) offsetof(s2dVertex, colour)); // offset
    
    // ebo data
    u32* indices = (u32*) malloc(S2D_MAX_INDICES * sizeof(u32));
    u32 offset = 0;
    for (u32 i = 0; i < S2D_MAX_INDICES; i += 6 ) {
        indices[i + 0] = offset + 0; 
        indices[i + 1] = offset + 1; 
        indices[i + 2] = offset + 2; 
        indices[i + 3] = offset + 2; 
        indices[i + 4] = offset + 3; 
        indices[i + 5] = offset + 0; 
        offset += 4;
    }

    // gen ebo
    glGenBuffers(1, &renderer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            S2D_MAX_INDICES * sizeof(u32),
            indices, 
            GL_STATIC_DRAW); 

    free(indices);
}


QuadRenderer quad_renderer_create() {

    _QuadRenderer* ren = malloc(sizeof(_QuadRenderer));

    init_buffers(ren);

    ren->currentVertex = ren->vertices;
    ren->indicesCount  = 0u;
    ren->verticesCount = 0u;
    ren->lastShader    = 0u;
    ren->lastTexture   = 0u;
    ren->drawCalls     = 0u;

    return (QuadRenderer) ren;
}


void quad_renderer_shutdown(QuadRenderer renderer) {
    free(renderer);
}


void quad_renderer_flush(QuadRenderer renderer) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    // don't bother if the buffer is empty
    if (ren->verticesCount == 0) {
        return;
    }

    glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            ren->verticesCount * sizeof(s2dVertex),
            ren->vertices);

    glDrawElements(GL_TRIANGLES, ren->indicesCount, GL_UNSIGNED_INT, 0);

    ren->verticesCount = 0u;
    ren->indicesCount  = 0u;
    ren->currentVertex = ren->vertices;
    ren->drawCalls++;
}


void quad_renderer_submit_quad(
        QuadRenderer renderer,
        clmVec2      position, 
        clmVec2      size, 
        clmVec4      colour,
        u32          texID,  
        s2dFrame     texSubRegion,
        u32          shader) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    // flush upon vertex buffer full, texture change, or shader change
    bool flush         = false;
    bool changeTexture = false;
    bool changeShader  = false;

    if (ren->verticesCount == S2D_MAX_VERTICES) {
        flush = true;
    }

    if (ren->lastTexture != texID) {
        flush = true;
        changeTexture = true;
    }

    if (ren->lastShader != shader) {
        flush = true;
        changeShader = true;
    }

    if (flush) {
        quad_renderer_flush(renderer);

        if (changeTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texID);
        }

        if (changeShader) {
            s2d_shader_set_uniform_1i(shader, "uTexture", 0);
            s2d_shader_use(shader);
        }
    }


    // bottom-left
    ren->currentVertex->position.x = position.x;
    ren->currentVertex->position.y = position.y;
    ren->currentVertex->texCoord.x = texSubRegion.x;
    ren->currentVertex->texCoord.y = texSubRegion.y;
    ren->currentVertex->colour     = colour;
    ren->currentVertex++;

    // bottom-right
    ren->currentVertex->position.x = position.x + size.x;
    ren->currentVertex->position.y = position.y;
    ren->currentVertex->texCoord.x = texSubRegion.x + texSubRegion.w;
    ren->currentVertex->texCoord.y = texSubRegion.y;
    ren->currentVertex->colour     = colour;
    ren->currentVertex++;

    // top-right
    ren->currentVertex->position.x = position.x + size.x;
    ren->currentVertex->position.y = position.y + size.y;
    ren->currentVertex->texCoord.x = texSubRegion.x + texSubRegion.w;
    ren->currentVertex->texCoord.y = texSubRegion.y + texSubRegion.h;
    ren->currentVertex->colour     = colour;
    ren->currentVertex++;

    // top-left
    ren->currentVertex->position.x = position.x;
    ren->currentVertex->position.y = position.y + size.y;
    ren->currentVertex->texCoord.x = texSubRegion.x;
    ren->currentVertex->texCoord.y = texSubRegion.y + texSubRegion.h;
    ren->currentVertex->colour     = colour;
    ren->currentVertex++;

    // increment counts and update the last shader used
    ren->indicesCount  += 6;
    ren->verticesCount += 4;
    ren->lastShader     = shader;
    ren->lastTexture    = texID;
}


void quad_renderer_print_stats(QuadRenderer renderer) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    printf("-------------------\n"
           "Quad Renderer Stats\n"
           "draw calls: %u\n"
           "-------------------\n",
           ren->drawCalls);
}


void quad_renderer_reset_stats(QuadRenderer renderer) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    ren->drawCalls = 0u;
}
