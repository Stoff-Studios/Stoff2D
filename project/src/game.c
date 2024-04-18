#include <stoff2d.h>
#include <systems.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    bool running;
    u32 mcTex;
    ParticleData fireParticles;
    ParticleData bloodParticles;
    ParticleData waterParticles;
    ParticleData partyParticles;
    ParticleData smokeParticles;
    f32 camSpeed;
    f32 camZoom;
} GameData;

GameData gData;

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

    // Move camera.
    if (s2d_keydown(S2D_KEY_W)) {
        s2d_camera_move(0, gData.camSpeed * timeStep);
    }
    if (s2d_keydown(S2D_KEY_A)) {
        s2d_camera_move(-gData.camSpeed * timeStep, 0);
    }
    if (s2d_keydown(S2D_KEY_S)) {
        s2d_camera_move(0, -gData.camSpeed * timeStep);
    }
    if (s2d_keydown(S2D_KEY_D)) {
        s2d_camera_move(gData.camSpeed * timeStep, 0);
    }

    // Spawn particles.
    if (s2d_keydown(S2D_KEY_P)) {
        gData.fireParticles.position = s2d_mouse_world_pos();
        s2d_particles_add(&gData.fireParticles);
    }
    if (s2d_keydown(S2D_KEY_L)) {
        gData.bloodParticles.position = s2d_mouse_world_pos();
        s2d_particles_add(&gData.bloodParticles);
    }
    if (s2d_keydown(S2D_KEY_K)) {
        gData.waterParticles.position = s2d_mouse_world_pos();
        s2d_particles_add(&gData.waterParticles);
    }
    if (s2d_keydown(S2D_KEY_J)) {
        gData.partyParticles.position = s2d_mouse_world_pos();
        s2d_particles_add(&gData.partyParticles);
    }
    if (s2d_keydown(S2D_KEY_M)) {
        gData.smokeParticles.position = s2d_mouse_world_pos();
        s2d_particles_add(&gData.smokeParticles);
    }

    // Systems.
    system_move(timeStep);
    system_render();
}

void test_entity(f32 x, f32 y, f32 vx, f32 vy) {
    u32 eID = s2d_ecs_create_entity();
    printf("creating entity with eID: %u\n", eID);
    Component position = (Component) {
        .eID = eID,
        .type = CMP_TYPE_POSITION,
        .position = (PositionComponent) {
            .position = (clmVec2) { x, y } 
        }
    };
    Component velocity = (Component) {
        .eID = eID,
        .type = CMP_TYPE_VELOCITY,
        .velocity = (VelocityComponent) {
            .velocity = (clmVec2) { vx, vy } 
        }
    };
    Component sprite = (Component) {
        .eID = eID,
        .type = CMP_TYPE_SPRITE,
        .sprite = (SpriteComponent) {
            .size = (clmVec2) { 128.0f, 128.0f },
            .texture = gData.mcTex,
            .frame = (Frame) { 0.0f, 0.0f, 1.0f, 1.0f },
            .layer = 0
        }
    };

    s2d_ecs_add_component(position);
    s2d_ecs_add_component(velocity);
    s2d_ecs_add_component(sprite);
}

void game_init() {
    if (!s2d_initialise_engine("Paralysis")) {
        fprintf(stderr, "Could not initialise stoff2d\n");
        exit(1);
    }
    s2d_set_flags(S2D_LOG_STATS);

    gData.mcTex    = s2d_load_texture("font.png");
    gData.camZoom  = 500.0f;
    gData.camSpeed = 300.0f;

    int n = 100;
    for (int i = 0; i < n; i++) {
        test_entity(i * 132.0f, 0.0f, i * 20, i * 20);
    }

    s2d_ecs_print_components();

    // Particles test.
    gData.fireParticles = (ParticleData) {
        .count    = 1,
        .lifeTime = 1.0f,
        .position      = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -20.0f, -5.0f },
        .upperVelocity = (clmVec2) {  20.0f, 50.0f },
        .lowerSize     = (clmVec2) { 1.0f, 1.0f },
        .upperSize     = (clmVec2) { 2.0f, 2.0f },
        .birthColour   = (clmVec4) { 0.8f, 0.0f, 0.0f, 1.0f },
        .deathColour   = (clmVec4) { 0.0f, 0.0f, 0.0f, 0.0f }
    };

    gData.bloodParticles = (ParticleData) {
        .count    = 1,
        .lifeTime = 0.5f,
        .position      = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -50.0f, -50.0f },
        .upperVelocity = (clmVec2) {  50.0f, 50.0f },
        .lowerSize     = (clmVec2) { 0.5f, 0.5f },
        .upperSize     = (clmVec2) { 1.0f, 1.0f },
        .birthColour   = (clmVec4) { 0.4f, 0.0f, 0.0f, 1.0f },
        .deathColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f }
    };

    gData.waterParticles = (ParticleData) {
        .count    = 1,
        .lifeTime = 1.0f,
        .position      = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -2.0f, -20.0f },
        .upperVelocity = (clmVec2) {  2.0f, 0.0f },
        .lowerSize     = (clmVec2) { 1.0f, 1.0f },
        .upperSize     = (clmVec2) { 2.0f, 2.0f },
        .birthColour   = (clmVec4) { 0.0f, 0.0f, 0.5f, 1.0f },
        .deathColour   = (clmVec4) { 0.0f, 0.0f, 1.0f, 0.0f }
    };

    gData.partyParticles = (ParticleData) {
        .count    = 2,
        .lifeTime = 2.0f,
        .position      = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -50.0f, -50.0f },
        .upperVelocity = (clmVec2) {  50.0f, 50.0f },
        .lowerSize     = (clmVec2) { 1.0f, 1.0f },
        .upperSize     = (clmVec2) { 50.0f, 50.0f },
        .birthColour   = (clmVec4) { 1.0f, 0.0f, 1.0f, 1.0f },
        .deathColour   = (clmVec4) { 0.0f, 1.0f, 1.0f, 0.0f }
    };

    gData.smokeParticles = (ParticleData) {
        .count    = 2,
        .lifeTime = 1.5f,
        .position      = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -50.0f, -2.0f },
        .upperVelocity = (clmVec2) {  50.0f, 20.0f },
        .lowerSize     = (clmVec2) { 2.0f, 2.0f },
        .upperSize     = (clmVec2) { 5.0f, 5.0f },
        .birthColour   = (clmVec4) { 0.15f, 0.1f, 0.1f, 1.0f },
        .deathColour   = (clmVec4) { 0.8f, 0.8f, 0.8f, 0.0f }
    };

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
