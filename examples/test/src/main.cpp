#include <stoff2d_core.h>

int main() {
    bool running = s2d_initialise_engine("test");

    s2d_set_flags(S2D_LOG_STATS);

    u32 tex1 = s2d_load_texture("skeleton_die.png");
    u32 tex2 = s2d_load_texture("player_idle.png");
    u32 tex3 = s2d_load_texture("skeleton_walk.png");

    s2dParticleType fireParticles;
    fireParticles.count = 100;
    fireParticles.lowerLifeTime = 0.1f;
    fireParticles.upperLifeTime = 2.0f;
    fireParticles.velocityRange = (clmVec2) { 2.0f, 500.0f };
    fireParticles.directionRange = (clmVec2) { 0.f, 2 * CLM_PI };
    fireParticles.lowerSize     = 2;
    fireParticles.upperSize     = 48;
    fireParticles.birthColour   = { 1.0f, 0.0f, 0.0f, 1.0f };
    fireParticles.deathColour   = { 1.0f, 0.0f, 0.0f, 1.0f };
    fireParticles.shader        = s2d_get_quad_shader();
    fireParticles.spriteName    = "circle_05";

    while (running) {
        s2d_start_frame();
        if (s2d_keydown(S2D_KEY_Q)) {
            running = false;
        }

        s2dSprite spr;

        spr.position = { -200.f, 50.f };
        spr.size     = { 32.f, 288.f };
        spr.colour   = { 1.f, 1.f, 1.f, 1.f };
        spr.texture  = tex2;
        spr.frame    = { 0.f, 0.f, 1.f, 1.f };
        spr.layer    = 0;
        spr.shader   = s2d_get_quad_shader();
        s2d_sprite_renderer_add_sprite(spr);

        spr.position = { -200.f, 0.f };
        spr.size     = { 495.f, 32.f };
        spr.colour   = { 1.f, 1.f, 1.f, 1.f };
        spr.texture  = tex1;
        spr.frame    = { 0.f, 0.f, 1.f, 1.f };
        spr.layer    = 0;
        spr.shader   = s2d_get_quad_shader();
        s2d_sprite_renderer_add_sprite(spr);

        s2d_particles_add(&fireParticles, s2d_mouse_world_pos());

        s2d_sprite_renderer_render_sprites();

        s2d_particles_render();

        s2d_text_render(
                "zerovelo",
                {},
                { 1.f, 1.f, 1.f, 1.f },
                1.f,
                0,
                s2d_get_text_shader(),
                "Stoff2D");

        s2d_sprite_renderer_render_sprites();

        s2d_end_frame();
    }

    return 0;
}
