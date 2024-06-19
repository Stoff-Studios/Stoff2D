#include <stoff2d_core.h>
#include <cds/cds_exlist.h>

// Extensible list of sprites to render each frame.
cdsExList* sprites;

int compare_sprite(const void* sprite1, const void* sprite2) {
    s2dSprite spr1 = (*(s2dSprite*) sprite1);
    s2dSprite spr2 = (*(s2dSprite*) sprite2);
    // Reversed ordering, we want 0 to be the front layer, i.e rendered last.
    return (spr2.layer - spr1.layer);
}

void s2d_sprite_renderer_add_sprite(s2dSprite sprite) {
    cds_exlist_push(sprites, &sprite);
}

void s2d_sprite_renderer_render_sprites() {
    cds_exlist_sort(sprites);
    for (size_t i = 0; i < cds_exlist_len(sprites); i++) {
        s2dSprite* sprite = (s2dSprite*) cds_exlist_get(sprites, i);
        if (sprite->texture == S2D_COLOURED_QUAD_TEXTURE) {
            s2d_render_coloured_quad(
                    sprite->position, 
                    sprite->size,
                    sprite->colour);
        } else {
            s2d_render_quad(
                    sprite->position,
                    sprite->size,
                    sprite->colour,
                    sprite->texture,
                    sprite->frame);
        }
    }
    cds_exlist_clear(sprites);
}

void sprite_renderer_init() {
    sprites = cds_exlist_create(
            sizeof(s2dSprite), 
            compare_sprite);
}

void sprite_renderer_shutdown() {
    cds_exlist_destroy(sprites);
}
