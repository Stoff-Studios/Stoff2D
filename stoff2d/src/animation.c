#include <animation.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 100
#define MAX_ENTRIES (S2D_MAX_ANIMATION_TABLE_SIZE / 2)


typedef struct {
    // Key.
    char name[S2D_MAX_ANIMATION_NAME_LEN];
    // Value.
    Animation animation;

    // Probing.
    bool empty;
} HashEntry;

HashEntry animations[S2D_MAX_ANIMATION_TABLE_SIZE];
u64       animationsCount = 0;

// djb2 hash.
u64 hash_str(char* key) {
    u64 hash = 5381;
    char c;
    while ((c = *key++) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool is_blank_or_comment(char* line) {
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

bool is_new_sprite_sheet(char* line) {
    return !strcmp("SPRITESHEET\n", line);
}

bool is_start_animation(char* line) {
    return line[0] == '{';
}

bool is_end_animation(char* line) {
    return line[0] == '}';
}

// print frame for debugging.
void print_frame(Frame* frame) {
    printf("Frame(%.2f, %.2f, %.2f, %.2f)", 
            frame->x, frame->y, frame->w, frame->h);
}

// print animation for debugging.
void print_animation(Animation* animation) {
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
    for (u32 i = 0; i < S2D_MAX_ANIMATION_TABLE_SIZE; i++) {
        HashEntry* entry = &animations[i];
        printf("%u --- name: %s empty: %d\n",
                i, entry->name, entry->empty);
    }
    printf("TABLE END\n");
}

void parse_ani_file() {
    FILE* aniFile = fopen(S2D_ANIMATION_FILE, "r");
    if (!aniFile) {
        fprintf(stderr, 
                "Could not open animation file\n");
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
            Animation animation;
            animation.frameCount = 0;
            while(fgets(line, LINE_BUFFER_SIZE, aniFile)) {
                if (!is_blank_or_comment(line)) {
                    if (is_end_animation(line)) {
                        break;
                    }
                    f32 x = atof(strtok(line, ","));
                    f32 y = atof(strtok(NULL, ","));
                    Frame frame = (Frame) {
                        .x = x * spriteW,
                        .y = y * spriteH,
                        .w = spriteW,
                        .h = spriteH
                    };
                    animation.frames[animation.frameCount++] = frame;
                }
            }
            s2d_animations_put(name, animation);
        }
    }
    fclose(aniFile);
}

void s2d_animations_init() {
    // Set each entry to be an empty slot.
    for (u32 i = 0; i < S2D_MAX_ANIMATION_TABLE_SIZE; i++) {
        HashEntry* entry = &animations[i];
        strcpy(entry->name, "NONE");
        memset(&entry->animation, 0, sizeof(Animation));
        entry->empty = true;
    }
    parse_ani_file();
}

void s2d_animations_put(char* name, Animation animation) {
    // Don't allow load factor to creep over 50%. 
    if (animationsCount == MAX_ENTRIES) {
        fprintf(stderr, 
                "Exceeded animation limit of %d when trying to add animation:"
                " %s, increase MAX_ANIMATION_TABLE_SIZE to a higher prime number"
                " in settings.h if you need more space for animations\n",
                MAX_ENTRIES, name);
        return;
    }

    u64 hash  = hash_str(name);
    u64 index = hash % S2D_MAX_ANIMATION_TABLE_SIZE;
    HashEntry* entry = &animations[index];

    // Quadratic probe until we find an empty slot.
    u64 probe = 1;
    while(!entry->empty) {
        index = (hash + (probe * probe)) % S2D_MAX_ANIMATION_TABLE_SIZE;
        entry = &animations[index];
        probe++;
    }

    // Insert the data into the free slot.
    strcpy(entry->name, name);
    memcpy(&entry->animation, &animation, sizeof(Animation));
    entry->empty = false;
    animationsCount++;
}

Animation* s2d_animations_get(char* name) {
    // Quadratic probe until we find the animation.
    HashEntry* entry;
    u64 index;
    u64 hash       = hash_str(name);
    u64 probe      = 0;
    while(true) {
        index = (hash + (probe * probe)) % S2D_MAX_ANIMATION_TABLE_SIZE;
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
