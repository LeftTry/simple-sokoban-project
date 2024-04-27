#include "raylib.h"

#include "globals.h"
#include "graphics.h"
#include "sounds.h"
#include "merge.h"

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Sokoban");
    SetTargetFPS(60);
    HideCursor();
    merge merge;
    load_fonts();
    load_images();
    load_sounds();
    merge.load_next_level();

    while (!WindowShouldClose()) {
        BeginDrawing();

        merge.update_game();
        merge.draw_game();

        EndDrawing();
    }
    CloseWindow();

    unload_sounds();
    unload_images();
    unload_fonts();


    return 0;
}
