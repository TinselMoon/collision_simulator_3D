#ifndef PARTICLE_H
#define PARTICLE_H
#include "particles.h"
#endif

int main(int argc, char **argv){
    if(argc < 3){
        printf("Error, not enough information\nRun: %s [width] [height] [num of particles]\n", argv[0]);
        exit(0);
    }
    srand(time(NULL));
    const int screenWidth = atoi(argv[1]);
    const int screenHeight = atoi(argv[2]);
    System *s = create();
    int num_particles = atoi(argv[3]);

    // Creating camera
    Box caixa = {100, 100, 100};

    // Config camera
    float cameraAngle = 0.0f;
    float cameraDistance = 200.0f;
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 100.0f, 100.0f };
    camera.target = (Vector3){ caixa.x/2, caixa.y/2, caixa.z/2 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.position.x = caixa.x/2 + cosf(cameraAngle) * cameraDistance;
    camera.position.y = caixa.y/2 + 100.0f;
    camera.position.z = caixa.z/2 + sinf(cameraAngle) * cameraDistance;
    
    for(int i = 0; i < num_particles; i++)
        insert_particle(s, caixa);

    InitWindow(screenWidth, screenHeight, "colisions simulator");
    SetTargetFPS(TARGET_FPS); //FPS value on config.h
    double fps;
    ClearBackground(RAYWHITE);

    while (!WindowShouldClose())    //Detect close button ESC
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            cameraAngle += GetMouseDelta().x * 0.01f;
        }

        camera.position.x = caixa.x/2 + cosf(cameraAngle) * cameraDistance;
        camera.position.z = caixa.z/2 + sinf(cameraAngle) * cameraDistance;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (IsKeyPressed(KEY_I)){
            insert_particle(s, caixa);
            num_particles++;
        }
        BeginMode3D(camera);
        if(!is_empty(s)){
            update_pos(s);
            for(int i = 0; i < 4; i++)
                fix_contacts(s, caixa);
            draw_particles(s);
            if (IsKeyPressed(KEY_R)){
                remove_particle(s);
                num_particles--;
            }
        }
        DrawCubeWires((Vector3){caixa.x/2,caixa.y/2,caixa.z/2}, caixa.x, caixa.y, caixa.z, DARKPURPLE);
        EndMode3D();
        fps = 1/GetFrameTime();
        DrawText(TextFormat("Number of Particles: %d\n"
                            "Total kinect energy: %.0f\n"
                            "Press I to insert a particle\n"
                            "Press R to remove a particle\n"
                            "FPS: %.2f", num_particles, kinect_energy(s), fps), 10, 10, 30, DARKGRAY);
        EndDrawing();
        EndDrawing();
    }

    CloseWindow();
    destroy(s);
    return 0;
}
