
#include <quad_renderer.h>

#include <glad/glad.h>
#include <shader.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define MAX_TEXTURE_SLOTS  32
#define EMPTY_TEXTURE_SLOT -1

typedef struct {
    // opengl buffer objects
    u32 vao;
    u32 vbo;
    u32 ebo;

    // vertex buffer
    s2dVertex  vertices[S2D_MAX_VERTICES];
    s2dVertex* currentVertex;

    // most recently used shader
    u32 lastShader;

    // batch counts
    u32 indicesCount;
    u32 verticesCount;

    // texture slots
    i32 textures[MAX_TEXTURE_SLOTS];
    i32 nextAvailableTexSlot;
    i32 userSetTexSlotCount;

    // an array of integers from 0 to MAX_TEXTURE_SLOTS - 1, these get sent
    // to the uTextures uniform upon new shader
    i32 samplers[MAX_TEXTURE_SLOTS];

    // maximum texture units available (usually 32 but varies per system)
    i32 maxTexSlots;

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
    
    // attribute 3, texSlot
    glEnableVertexAttribArray(3);  
    glVertexAttribPointer(
            3,                                     // attribute no.
            1,                                     // number of elements
            GL_FLOAT,                              // data type of the elements
            GL_FALSE,                              // normalise
            sizeof(s2dVertex),                     // stride
            (void*) offsetof(s2dVertex, texSlot)); // offset

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

    ren->currentVertex        = ren->vertices;
    ren->indicesCount         = 0u;
    ren->verticesCount        = 0u;
    ren->lastShader           = 0xffffffff;
    ren->drawCalls            = 0u;
    ren->userSetTexSlotCount  = 0;
    ren->nextAvailableTexSlot = 1;
    memset(ren->textures, 0, sizeof(ren->textures));
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &ren->maxTexSlots);

    for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        ren->samplers[i] = i;
    }

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

    glBindVertexArray(ren->vao);
    glBindVertexArray(ren->vbo);
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

    // flush upon vertex buffer full
    if (ren->verticesCount == S2D_MAX_VERTICES) {
        quad_renderer_flush(renderer);
    }

    // also flush if the shader changed and set up samplers
    if (ren->lastShader != shader) {
        quad_renderer_flush(renderer);
        s2d_shader_use(shader);
        i32 loc = glGetUniformLocation(shader, "uTextures");
        glUniform1iv(loc, MAX_TEXTURE_SLOTS, ren->samplers);
    }

    // see if this texture is already bound in this batch
    f32 texSlot = EMPTY_TEXTURE_SLOT;
    for (i32 slot = 0; slot < MAX_TEXTURE_SLOTS; slot++) {
        if (ren->textures[slot] == texID) {
            texSlot = (f32) slot;
            break;
        }
    }

    // if the texture isn't bound yet, need to assign it a slot
    if (texSlot == EMPTY_TEXTURE_SLOT) {
        // if there are no more texture slots left, need to flush
        // and reset texture slots not set by the user
        if (ren->nextAvailableTexSlot >= MAX_TEXTURE_SLOTS) {
            quad_renderer_flush(renderer);
            for (int slot = ren->userSetTexSlotCount + 1; 
                    slot < MAX_TEXTURE_SLOTS; slot++) {
                ren->textures[slot] = 0;
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            ren->nextAvailableTexSlot = ren->userSetTexSlotCount + 1;
        }

        // assign the new texture a slot
        texSlot = (f32) ren->nextAvailableTexSlot;
        ren->textures[ren->nextAvailableTexSlot] = texID;
        glActiveTexture(GL_TEXTURE0 + ren->nextAvailableTexSlot);
        glBindTexture(GL_TEXTURE_2D, texID);
        ren->nextAvailableTexSlot++;
    }

    // bottom-left
    ren->currentVertex->position.x = position.x;
    ren->currentVertex->position.y = position.y;
    ren->currentVertex->texCoord.x = texSubRegion.x;
    ren->currentVertex->texCoord.y = texSubRegion.y;
    ren->currentVertex->colour     = colour;
    ren->currentVertex->texSlot    = texSlot;
    ren->currentVertex++;

    // bottom-right
    ren->currentVertex->position.x = position.x + size.x;
    ren->currentVertex->position.y = position.y;
    ren->currentVertex->texCoord.x = texSubRegion.x + texSubRegion.w;
    ren->currentVertex->texCoord.y = texSubRegion.y;
    ren->currentVertex->colour     = colour;
    ren->currentVertex->texSlot    = texSlot;
    ren->currentVertex++;

    // top-right
    ren->currentVertex->position.x = position.x + size.x;
    ren->currentVertex->position.y = position.y + size.y;
    ren->currentVertex->texCoord.x = texSubRegion.x + texSubRegion.w;
    ren->currentVertex->texCoord.y = texSubRegion.y + texSubRegion.h;
    ren->currentVertex->colour     = colour;
    ren->currentVertex->texSlot    = texSlot;
    ren->currentVertex++;

    // top-left
    ren->currentVertex->position.x = position.x;
    ren->currentVertex->position.y = position.y + size.y;
    ren->currentVertex->texCoord.x = texSubRegion.x;
    ren->currentVertex->texCoord.y = texSubRegion.y + texSubRegion.h;
    ren->currentVertex->colour     = colour;
    ren->currentVertex->texSlot    = texSlot;
    ren->currentVertex++;

    // increment counts and update the last shader used
    ren->indicesCount  += 6;
    ren->verticesCount += 4;
    ren->lastShader     = shader;
}


void quad_renderer_print_stats(QuadRenderer renderer) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    printf("-------------------\n"
           "Quad Renderer Stats\n"
           "draw calls: %u\n"
           "user set tex slot count: %d\n"
           "next available tex slot: %d\n"
           "max texture slots (only using 32): %d\n"
           "-------------------\n",
           ren->drawCalls,
           ren->userSetTexSlotCount,
           ren->nextAvailableTexSlot,
           ren->maxTexSlots);
}


void quad_renderer_reset_stats(QuadRenderer renderer) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    ren->drawCalls = 0u;
}


void quad_renderer_set_texture_slots(
        QuadRenderer renderer,
        u32 count,
        u32 texIDs[]) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    // need to save one automatic slot at the end
    if (count > MAX_TEXTURE_SLOTS - 2) {
        fprintf(stderr, "[S2D Warning] tried to bind more than 30 textures\n");
        return;
    }

    quad_renderer_reset_texture_slots(renderer);

    // bind user set textures in order
    for (i32 slot = 0; slot < count; slot++) {
        glActiveTexture(GL_TEXTURE0 + slot + 1);
        glBindTexture(GL_TEXTURE_2D, texIDs[slot]);
        ren->textures[slot + 1] = texIDs[slot];
    }

    ren->nextAvailableTexSlot = count + 1;
    ren->userSetTexSlotCount  = count;
}


void quad_renderer_reset_texture_slots(QuadRenderer renderer) {

    _QuadRenderer* ren = (_QuadRenderer*) renderer;

    for (i32 slot = 0; slot < MAX_TEXTURE_SLOTS; slot++) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
        ren->textures[slot] = 0;
    }

    ren->userSetTexSlotCount  = 0;
    ren->nextAvailableTexSlot = 1;
}
