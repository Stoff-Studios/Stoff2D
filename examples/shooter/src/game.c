#include <stoff2d.h>
#include <systems.h>
#include <entities.h>
#include <particle_types.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    bool running;
    u32  texFont;
    f32  camSpeed;
    f32  camZoom;
    u32  playerEID;
} GameData;

static GameData gData;

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

    // Systems.
    system_control();
    system_move(timeStep);
    system_death_timer(timeStep);
    system_particles(timeStep);
    system_render();
}

void game_init() {
    if (!s2d_initialise_engine("Paralysis")) {
        fprintf(stderr, "Could not initialise stoff2d\n");
        exit(1);
    }
    s2d_set_flags(S2D_LOG_STATS);
    particle_types_init();
    gData.texFont  = s2d_load_texture("font.png");
    gData.camZoom  = 500.0f;
    gData.camSpeed = 300.0f;
    gData.playerEID = create_player((clmVec2) { 0.0f, 0.0f });
    gData.running = true;
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
}
