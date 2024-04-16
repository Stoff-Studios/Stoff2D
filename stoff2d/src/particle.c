#include <particle.h>

#include <stoff2d.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    bool    active;
    f32     birthTime;
    f32     currentDuration;
    f32     lifeTime;
    clmVec2 position;
    clmVec2 velocity;
    clmVec2 size;
    clmVec4 birthColour;
    clmVec4 colourChange;
    clmVec4 currentColour;
} Particle;

Particle particles[MAX_PARTICLES];
u64      nextIndex = 0;
u64      aliveCount = 0;

void particles_init() {
    srand(12345678);
    memset(particles, 0, sizeof(Particle) * MAX_PARTICLES);
}

// Return a random float in the range [0, 1]
f32 randf() {
    return ((f32) rand() / (f32) RAND_MAX);
}

void s2d_particles_add(ParticleData* pData) {
    clmVec2 velVariation = clm_v2_add(
            pData->upperVelocity, 
            clm_v2_scalar_mul(-1, pData->lowerVelocity)
            );
    clmVec2 sizeVariation = clm_v2_add(
            pData->upperSize, 
            clm_v2_scalar_mul(-1, pData->lowerSize)
            );
    clmVec4 colourChange = (clmVec4) {
        .r = pData->deathColour.r - pData->birthColour.r,
        .g = pData->deathColour.g - pData->birthColour.g,
        .b = pData->deathColour.b - pData->birthColour.b,
        .a = pData->deathColour.a - pData->birthColour.a
    };
    for (u32 i = 0; i < pData->count; i++) {
        u64 pIndex = nextIndex++;
        nextIndex = nextIndex % MAX_PARTICLES;
        Particle* particle = &particles[pIndex];
        if (!particle->active) {
            particle->active = true;
            aliveCount++;
        }
        particle->birthTime = glfwGetTime();
        particle->currentDuration = 0.0f;
        particle->lifeTime = pData->lifeTime;
        particle->position = pData->position;
        particle->velocity = (clmVec2) {
            pData->lowerVelocity.x + (velVariation.x * randf()),
            pData->lowerVelocity.y + (velVariation.y * randf())
        };
        particle->size = (clmVec2) {
            pData->lowerSize.x + (sizeVariation.x * randf()),
            pData->lowerSize.y + (sizeVariation.y * randf())
        };
        particle->birthColour = pData->birthColour;
        particle->currentColour = pData->birthColour;
        particle->colourChange = colourChange;
    }
}

void particles_render() {
    u64 renderedCount = 0;
    for (u64 i = 0; i < MAX_PARTICLES; i++) {
        Particle p = particles[i];
        if (renderedCount == aliveCount) {
            break;
        }
        if (p.active) {
            s2d_render_coloured_quad(
                    p.position,
                    p.size,
                    p.currentColour);
            renderedCount++;
        }
    }
}

void particles_update(f32 timeStep) {
    u64 updatedCount = 0;
    u64 diedCount = 0;
    for (u64 i = 0; i < MAX_PARTICLES; i++) {
        Particle* p = &particles[i];
        if (updatedCount == aliveCount) {
            break;
        }
        if (p->active) {
            updatedCount++;
            p->currentDuration += timeStep;
            if (p->currentDuration >= p->lifeTime) {
                p->active = false;
                diedCount++;
            }
            // Update position.
            clmVec2 moveBy = clm_v2_scalar_mul(timeStep, p->velocity);
            p->position = clm_v2_add(p->position, moveBy);
            // Update colour.
            f32 interpolation = p->currentDuration / p->lifeTime;
            p->currentColour.r = 
                p->birthColour.r + (p->colourChange.r * interpolation);
            p->currentColour.g = 
                p->birthColour.g + (p->colourChange.g * interpolation);
            p->currentColour.b = 
                p->birthColour.b + (p->colourChange.b * interpolation);
            p->currentColour.a = 
                p->birthColour.a + (p->colourChange.a * interpolation);
        }
    }
    aliveCount -= diedCount;
}
