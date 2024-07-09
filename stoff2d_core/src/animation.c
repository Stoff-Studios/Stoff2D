#include <defines.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 1024
#define TABLE_SIZE (S2D_MAX_ANIMATIONS * 2)

typedef struct {
    // Key.
    char name[S2D_MAX_ANIMATION_NAME_LEN];
    // Value.
    s2dAnimation animation;

    // Probing.
    bool empty;
} HashEntry;

HashEntry animations[TABLE_SIZE];
u64       animationsCount = 0;


// djb2 hash.
u64 hash_str(const char* key) {
    u64 hash = 5381;
    char c;
    while ((c = *key++) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool is_blank_or_comment(const char* line) {
    char c;
    while((c = *line++) != '\n') {
        if (c == '#') {
            return true;
        }
        if (c != ' ') {
            return false;
        }
    }
    return true;
}

bool is_new_sprite_sheet(const char* line) {
    return !strcmp("SPRITESHEET\n", line);
}

bool is_start_animation(const char* line) {
    return line[0] == '{';
}

bool is_end_animation(const char* line) {
    return line[0] == '}';
}

// print frame for debugging.
void print_frame(s2dFrame* frame) {
    printf("Frame(%.2f, %.2f, %.2f, %.2f)", 
            frame->x, frame->y, frame->w, frame->h);
}

// print animation for debugging.
void print_animation(s2dAnimation* animation) {
    printf("ANIMATION START\n");
    printf("frameCount: %u\n", animation->frameCount);
    printf("frames: [");
    for (u32 i = 0; i < animation->frameCount - 1; i++) {
        print_frame(&animation->frames[i]);
        printf(", ");
    }
    print_frame(&animation->frames[animation->frameCount - 1]);
    printf("]\n");
    printf("ANIMATION END\n");
}

// print table for debugging.
void print_table() {
    printf("TABLE START\n");
    for (u32 i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = &animations[i];
        printf("%u --- name: %s empty: %d\n",
                i, entry->name, entry->empty);
    }
    printf("TABLE END\n");
}

void animations_put(const char* name, s2dAnimation animation) {
    // Don't allow load factor to creep over 50%. 
    if (animationsCount == S2D_MAX_ANIMATIONS) {
        fprintf(stderr, 
                "Exceeded animation limit of %d when trying to add animation:"
                " %s, increase MAX_ANIMATION_TABLE_SIZE to a higher prime number"
                " in settings.h if you need more space for animations\n",
                S2D_MAX_ANIMATIONS, name);
        return;
    }

    u64 hash  = hash_str(name);
    u64 index = hash % TABLE_SIZE;
    HashEntry* entry = &animations[index];

    // Quadratic probe until we find an empty slot.
    u64 probe = 1;
    while(!entry->empty) {
        index = (hash + (probe * probe)) % TABLE_SIZE;
        entry = &animations[index];
        probe++;
    }

    // Insert the data into the free slot.
    strcpy(entry->name, name);
    memcpy(&entry->animation, &animation, sizeof(s2dAnimation));
    entry->empty = false;
    animationsCount++;
}

void parse_ani_file() {
    FILE* aniFile = fopen(S2D_ANIMATION_FILE, "r");
    if (!aniFile) {
        fprintf(stderr, 
                "[S2D Error] could not open animation file - %s\n",
                S2D_ANIMATION_FILE);
        return;
    }

    f32 spriteW = 0.0f;
    f32 spriteH = 0.0f;
    char line[LINE_BUFFER_SIZE];
    while (fgets(line, LINE_BUFFER_SIZE, aniFile)) {
        if (is_blank_or_comment(line)) {
            continue;
        }
        if (is_new_sprite_sheet(line)) {
            fgets(line, LINE_BUFFER_SIZE, aniFile);
            f32 sheetW = atof(strtok(line, " "));
            f32 sheetH = atof(strtok(NULL, " "));
            spriteW = atof(strtok(NULL, " ")) / sheetW;
            spriteH = atof(strtok(NULL, " ")) / sheetH;
            continue;
        }
        if (is_start_animation(line)) {
            // Get the name.
            char name[S2D_MAX_ANIMATION_NAME_LEN];
            while(fgets(line, LINE_BUFFER_SIZE, aniFile)) {
                if (!is_blank_or_comment(line)) {
                    strncpy(name, line, S2D_MAX_ANIMATION_NAME_LEN);
                    // remove new line
                    for (u32 i = 0; i < S2D_MAX_ANIMATION_NAME_LEN; i++) {
                        if (name[i] == '\n') {
                            name[i] = '\0';
                        }
                    }
                    break;
                }
            }
            // Get the frames.
            s2dAnimation animation;
            animation.frameCount = 0;
            while(fgets(line, LINE_BUFFER_SIZE, aniFile)) {
                if (is_blank_or_comment(line)) {
                    continue;
                }
                if (is_end_animation(line)) {
                    break;
                }
                if (animation.frameCount == S2D_MAX_ANIMATION_FRAMES) {
                    fprintf(stderr,
                            "[S2D Error] tried to load animation longer than "
                            "limit of %d (dropped frames), "
                            "increase S2D_MAX_ANIMATION_FRAMES in settings.h\n",
                            S2D_MAX_ANIMATION_FRAMES);
                    break;
                }
                f32 x = atof(strtok(line, ","));
                f32 y = atof(strtok(NULL, ","));
                s2dFrame frame = (s2dFrame) {
                    .x = x * spriteW,
                    .y = y * spriteH,
                    .w = spriteW,
                    .h = spriteH
                };
                animation.frames[animation.frameCount++] = frame;
            }
            animations_put(name, animation);
        }
    }

    fclose(aniFile);
}

void animations_init() {
    // Set each entry to be an empty slot.
    for (u32 i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = &animations[i];
        strcpy(entry->name, "NONE");
        memset(&entry->animation, 0, sizeof(s2dAnimation));
        entry->empty = true;
    }
    parse_ani_file();
}

s2dAnimation* s2d_animations_get(const char* name) {
    // Quadratic probe until we find the animation.
    HashEntry* entry;
    u64 index;
    u64 hash       = hash_str(name);
    u64 probe      = 0;
    while(true) {
        index = (hash + (probe * probe)) % TABLE_SIZE;
        entry = &animations[index];

        // Ran into empty slot, stop the search. Don't have to worry about
        // the entire table being full since load factor is capped at 50%.
        if (entry->empty) {
            printf("No animation with name: %s found in animations.\n",
                    name);
            return NULL;
        }
        // Found it, return a reference to the animation.
        if (!strcmp(entry->name, name)) {
            return &entry->animation;
        }

        probe++;
    }

    // Unreachable.
    return NULL;
}
