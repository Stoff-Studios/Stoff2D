#include <stoff2d.h>
#include <cds/cds_exlist.h>

typedef struct {
    clmVec2         position;
    SpriteComponent sprite;
} Sprite;

// Extensible list of sprites to render each frame.
cdsExList* sprites;

int compare_sprite(const void* sprCmp1, const void* sprCmp2) {
    Sprite spr1 = (*(Sprite*) sprCmp1);
    Sprite spr2 = (*(Sprite*) sprCmp2);
    // Reversed ordering, we want 0 to be the front layer, i.e rendered last.
    return (*((Sprite*) sprCmp2)).sprite.layer - (*((Sprite*) sprCmp1)).sprite.layer;
}


void s2d_sprite_renderer_add_sprite(SpriteComponent sprite, clmVec2 position) {
    Sprite spr = (Sprite) {
        .position = position,
        .sprite = sprite
    };
    cds_exlist_push(sprites, &spr);
}

void s2d_sprite_renderer_render_sprites() {
    cds_exlist_sort(sprites);
    for (size_t i = 0; i < cds_exlist_len(sprites); i++) {
        Sprite* sprite = (Sprite*) cds_exlist_get(sprites, i);
        s2d_render_quad(
                sprite->position,
                sprite->sprite.size,
                (clmVec4) { 1.0f, 1.0f, 1.0f, 1.0f },
                sprite->sprite.texture,
                sprite->sprite.frame);
    }
    cds_exlist_clear(sprites);
}

void sprite_renderer_init() {
    sprites = cds_exlist_create(
            sizeof(Sprite), 
            compare_sprite);
}

void sprite_renderer_shutdown() {
    cds_exlist_destroy(sprites);
}
