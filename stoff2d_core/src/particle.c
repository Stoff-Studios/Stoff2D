#include <stoff2d_core.h>
#include <utils.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdlib.h>
#include <stbi/stbi_image.h>

#define RENDER_TEX_W 1024
#define RENDER_TEX_H 1024

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
    char    spriteName[32]; 
    u32     shader;
} Particle;

typedef struct {
    char* spriteName;
    u32   texture;
} ParticleSprite;

Particle particles[S2D_MAX_PARTICLES];
u64      nextIndex = 0;
u64      aliveCount = 0;

// look up particle sprite by name in here for texture subregion.
ParticleSprite* particleSprites;
u32             particleSpritesCount = 0;


u32 lookup_particle_texture(const char* spriteName) {
    ParticleSprite* p = particleSprites;
    for (u32 i = 0; i < particleSpritesCount; i++) {
        if (!strcmp(p->spriteName, spriteName)){
            return p->texture;
        }
        p++;
    }
    fprintf(stderr,
            "[S2D Error] could not find particle texture with name %s\n",
            spriteName);
    return 0;
}

void particles_init() {
    srand(12345678);
    memset(particles, 0, sizeof(Particle) * S2D_MAX_PARTICLES);

    char** files    = list_files_in_dir(S2D_PARTICLE_SPRITES_FOLDER);
    char** filesCpy = files; // NOTE: for freeing
    char*  fileName;         // NOTE: freed in shutdown
    while ((fileName = *files++)) {
        // filter out current and previous directory listing.
        if (!strcmp(".", fileName) || !strcmp("..", fileName)) {
            free(fileName);
            continue;
        } 

        // Construct path for texture.
        const char* textureDir = "particles/";
        size_t texPathLen = strlen(textureDir) + strlen(fileName);
        char* texPath = (char*) malloc((texPathLen * sizeof(char)) + 1);
        strcpy(texPath, textureDir);
        strcat(texPath, fileName);

        // load it.
        u32 texture = s2d_load_texture(texPath);
        free(texPath);

        // strip off .png extension and save that as key in lookup array.
        particleSprites = realloc(
                particleSprites, 
                sizeof(ParticleSprite) * ++particleSpritesCount);
        fileName[strlen(fileName) - 4] = '\0';
        particleSprites[particleSpritesCount - 1].spriteName = fileName;
        particleSprites[particleSpritesCount - 1].texture = texture;
    }

    free(filesCpy);
}

// Return a random float in the range [0, 1]
f32 randf() {
    return ((f32) rand() / (f32) RAND_MAX);
}

void s2d_particles_add(ParticleData* pData) {
    clmVec2 velVariation = clm_v2_add(
            pData->upperVelocity, 
            clm_v2_scalar_mul(-1, pData->lowerVelocity));

    clmVec2 sizeVariation = clm_v2_add(
            pData->upperSize, 
            clm_v2_scalar_mul(-1, pData->lowerSize));

    clmVec4 colourChange = (clmVec4) {
        .r = pData->deathColour.r - pData->birthColour.r,
        .g = pData->deathColour.g - pData->birthColour.g,
        .b = pData->deathColour.b - pData->birthColour.b,
        .a = pData->deathColour.a - pData->birthColour.a
    };

    for (u32 i = 0; i < pData->count; i++) {
        u64 pIndex = nextIndex++;
        nextIndex  = nextIndex % S2D_MAX_PARTICLES;

        Particle* particle = &particles[pIndex];
        if (!particle->active) {
            particle->active = true;
            aliveCount++;
        }

        particle->birthTime       = glfwGetTime();
        particle->currentDuration = 0.0f;
        particle->lifeTime        = pData->lifeTime;
        particle->position        = pData->position;
        particle->velocity        = (clmVec2) {
            pData->lowerVelocity.x + (velVariation.x * randf()),
            pData->lowerVelocity.y + (velVariation.y * randf())
        };
        particle->size = (clmVec2) {
            pData->lowerSize.x + (sizeVariation.x * randf()),
            pData->lowerSize.y + (sizeVariation.y * randf())
        };
        particle->birthColour   = pData->birthColour;
        particle->currentColour = pData->birthColour;
        particle->colourChange  = colourChange;
        strncpy(particle->spriteName, pData->spriteName, 32);
        particle->shader = pData->shader;
    }
}

void s2d_particles_render() {
    u64 renderedCount = 0;
    for (u64 i = 0; i < S2D_MAX_PARTICLES; i++) {
        Particle p = particles[i];

        if (renderedCount == aliveCount) {
            break;
        }

        if (p.active) {
            renderedCount++;
            s2d_render_quad(
                    p.position,
                    p.size,
                    p.currentColour,
                    lookup_particle_texture(p.spriteName),
                    S2D_ENTIRE_TEXTURE,
                    p.shader);
        }
    }
}

void particles_update(f32 timeStep) {
    u64 updatedCount = 0;
    u64 diedCount    = 0;
    for (u64 i = 0; i < S2D_MAX_PARTICLES; i++) {
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

void particles_shutdown() {
    ParticleSprite* p = particleSprites;
    for (u32 i = 0; i < particleSpritesCount; i++) {
        free(p->spriteName);
        p++;
    }
    free(particleSprites);
}

