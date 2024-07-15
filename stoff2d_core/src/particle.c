#include <stoff2d_core.h>
#include <utils.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdlib.h>
#include <stbi/stbi_image.h>

#include <math.h>

typedef struct {
    bool    active;

    f32     birthTime;
    f32     lifeTime;
    f32     currentDuration;

    clmVec2 position;
    clmVec2 velocity;
    clmVec2 size;

    clmVec4 birthColour;
    clmVec4 currentColour;
    clmVec4 colourChange;

    u32     shader;
    u32     texture;
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

void s2d_particles_add(const s2dParticleType* pData, clmVec2 position) {
    // precalculate the variations for this particle type.
    const clmVec2 velVariation = clm_v2_add(
            pData->upperVelocity, 
            clm_v2_scalar_mul(-1.0f, pData->lowerVelocity));
    const clmVec4 colourChange = (clmVec4) {
        .r = pData->deathColour.r - pData->birthColour.r,
        .g = pData->deathColour.g - pData->birthColour.g,
        .b = pData->deathColour.b - pData->birthColour.b,
        .a = pData->deathColour.a - pData->birthColour.a
    };
    const f32 sizeVariation = pData->upperSize - pData->lowerSize;
    const f32 lifeTimeVariation = fabs(
            pData->upperLifeTime - pData->lowerLifeTime);

    for (u32 i = 0; i < pData->count; i++) {
        // circular array
        u64 pIndex = nextIndex++;
        nextIndex  = nextIndex % S2D_MAX_PARTICLES;
        Particle* particle = &particles[pIndex];

        // only increment alive count if we are not overwriting an alive one
        if (!particle->active) {
            particle->active = true;
            aliveCount++;
        }

        // birthTime
        particle->birthTime = glfwGetTime();
        // lifeTime
        particle->lifeTime = pData->lowerLifeTime + (randf() * lifeTimeVariation),
        // alive duration
        particle->currentDuration = 0.0f;
        // randomly set the size within the variation specified
        f32 size = pData->lowerSize + (randf() * (f32) sizeVariation);
        particle->size = (clmVec2) { size, size };
        // starting position centered (varies by size)
        particle->position = clm_v2_add(
                position,
                clm_v2_scalar_mul(-0.5f, particle->size));
        // randomly set the velocity within variation specified
        particle->velocity = (clmVec2) {
            pData->lowerVelocity.x + (velVariation.x * randf()),
            pData->lowerVelocity.y + (velVariation.y * randf())
        };
        // current colour
        particle->currentColour = pData->birthColour;
        // colour variation
        particle->colourChange = colourChange;
        // texture
        particle->texture = lookup_particle_texture(pData->spriteName);
        // shader
        particle->shader = pData->shader;
        // birthColour
        particle->birthColour = pData->birthColour;
    }
}

void s2d_particles_render() {
    u64 renderedCount = 0;
    for (u64 i = 0; i < S2D_MAX_PARTICLES; i++) {
        if (renderedCount == aliveCount) {
            break;
        }
        Particle p = particles[i];
        if (p.active) {
            renderedCount++;
            s2d_render_quad(
                    p.position,
                    p.size,
                    p.currentColour,
                    p.texture,
                    S2D_ENTIRE_TEXTURE,
                    p.shader);
        }
    }
}

void particles_update(f32 timeStep) {
    u64 updatedCount = 0;
    u64 diedCount    = 0;
    for (u64 i = 0; i < S2D_MAX_PARTICLES; i++) {
        if (updatedCount == aliveCount) {
            break;
        }

        Particle* p = &particles[i];

        if (!p->active) {
            continue;
        }

        updatedCount++;
        p->currentDuration += timeStep;

        if (p->currentDuration >= p->lifeTime) {
            p->active = false;
            diedCount++;
            continue;
        }

        // update position.
        p->position = clm_v2_add(
                p->position,
                clm_v2_scalar_mul(timeStep, p->velocity));

        // Update colour.
        f32 interpolation = p->currentDuration / p->lifeTime;
        clmVec4 bc = p->birthColour;
        clmVec4 cc = p->colourChange;
        p->currentColour = (clmVec4) {
            .r = bc.r + (interpolation * cc.r),
            .g = bc.g + (interpolation * cc.g),
            .b = bc.b + (interpolation * cc.b),
            .a = bc.a + (interpolation * cc.a)
        };
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

