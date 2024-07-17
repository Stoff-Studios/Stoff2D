#include <game.h>
#include <stoff2d_core.h>
#include <stoff2d_ecs.h>
#include <systems.h>
#include <entities.h>
#include <particle_types.h>

#include <stdlib.h>
#include <stdio.h>

GameData gData;

void update_shaders() {
    s2d_shader_set_uniform_mat4(
            gData.canvasShader,
            "proj",
            s2d_camera_projection());
    s2d_shader_set_uniform_mat4(
            gData.canvasShader,
            "view",
            s2d_camera_view());
    s2d_shader_set_uniform_mat4(
            gData.screenShader,
            "proj",
            clm_mat4_ortho(
                0, 
                s2d_get_viewport_dimensions().x,
                0,
                s2d_get_viewport_dimensions().y,
                0.0f, 
                10.0f));
}

void game_update(f32 timeStep) {
    // Quit.
    if (s2d_keydown(S2D_KEY_Q)) {
        gData.running = false;
    }

    // Zoom.
    if (s2d_keydown(S2D_KEY_O)) {
        s2d_camera_zoom(gData.camZoom * timeStep);
    }
    if (s2d_keydown(S2D_KEY_I)) {
        s2d_camera_zoom(-gData.camZoom * timeStep);
    }

    // Toggle fullscreen/windowed.
    if (s2d_keydown(S2D_KEY_F)) {
        s2d_window_fullscreen();
    }
    if (s2d_keydown(S2D_KEY_V)) {
        s2d_window_windowed();
    }

    // Pause/Run.
    if (s2d_keydown(S2D_KEY_P)) {
        s2d_set_flags(S2D_PAUSED);
    } 
    if (s2d_keydown(S2D_KEY_R)) {
        s2d_unset_flags(S2D_PAUSED);
    } 

    // Toggle Hitboxes.
    if (s2d_keydown(S2D_KEY_H)) {
        gData.renderHitboxes = true;
    } else {
        gData.renderHitboxes = false;
    }

    update_shaders();
    system_control(timeStep);
    system_spawn_enemies(timeStep);
    system_enemy(timeStep);
    system_move(timeStep);
    system_move_hitboxes();
    system_death_timer(timeStep);
    system_invinsibility(timeStep);
    system_damage();
    system_animation(timeStep);
    system_particles(timeStep);
    system_render(timeStep);
}

void game_init() {
    if (!s2d_initialise_engine("Shooter")) {
        fprintf(stderr, "Could not initialise stoff2d\n");
        exit(1);
    }

    s2d_set_frame_cap(S2D_FPS_UNCAPPED);
    s2d_set_flags(S2D_LOG_STATS);

    entites_set_game_data_ptr(&gData);
    systems_set_game_data_ptr(&gData);
    particles_set_game_data_ptr(&gData);

    gData.texHitBox       = s2d_load_texture("hitbox.png");
    gData.texSkeletonWalk = s2d_load_texture("skeleton_walk.png");
    gData.texSkeletonDie  = s2d_load_texture("skeleton_die.png");
    gData.texPlayerIdle   = s2d_load_texture("player_idle.png");

    gData.camZoom  = 500.0f;
    gData.camSpeed = 300.0f;
    gData.running = true;
    gData.renderHitboxes = true;
    gData.paused = false;
    gData.shotTimer = 0.0f;
    gData.killCount = 0;

    gData.canvas = s2d_rendertexture_create(
            s2d_get_screen_dimensions().x,
            s2d_get_screen_dimensions().y,
            4,
            false);
    gData.canvasShader = s2d_shader_create("vCanvas.glsl", "fCanvas.glsl");
    gData.screenShader = s2d_shader_create("vScreen.glsl", "fScreen.glsl");

    s2d_ecs_initialise();
    particle_types_init();

    gData.playerEID = create_player((clmVec2) { 0.0f, 0.0f });
}

void game_run() {
    while (gData.running) {
        f32 timeStep = s2d_start_frame();
        game_update(timeStep);
        s2d_end_frame();
    }
}

void game_shutdown() {
    s2d_shutdown_engine();
    s2d_ecs_shutdown();
}
