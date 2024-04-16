#pragma once

#include <defines.h>
#include <clm/clm.h>

typedef struct {
    u32     count;         // number of particles to spawn.
    f32     lifeTime;      // lifetime of each particle in seconds.
    clmVec2 position;
    clmVec2 lowerVelocity; // lower velocity bound.
    clmVec2 upperVelocity; // upper velocity bound.
    clmVec2 lowerSize;     // lower size bound.
    clmVec2 upperSize;     // upper size bound.
    clmVec4 birthColour;   // start colour.
    clmVec4 deathColour;   // end colour.
} ParticleData;

/* s2d_particles_add
 * -----------------
 * Spawn a cluster of partcles with properties defined by
 * ParticleData.
 *
 * count determines how many particles are spawned.
 *
 * The velocity and size of each particle will be normally 
 * distributed between,
 * [lowerVelocity, upperVelocity] and
 * [lowerSize, upperSize] respectively.
 *
 * The colour of each particle will be linearly interpolated
 * between birthColour and deathColour over each particles lifeTime.
 */
void s2d_particles_add(ParticleData* particlesData);

/* particles_init
 * --------------
 *  Initalises particles array.
 * NOTE: This is called by the engine no need to call.
 */
void particles_init();

/* particles_render
 * ----------------
 * Renders active particles to the screen.
 * NOTE: This is called by the engine no need to call.
 */
void particles_render();

/* particles_update
 * ----------------
 * Updates active particles.
 * NOTE: This is called by the engine no need to call.
 */
void particles_update(f32 timeStep);
