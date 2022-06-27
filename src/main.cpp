#include "game/game.hpp"

int main(void) {
    Game_t game {};


    // Vector3f_t rot {0, 180, 0};
    // Vector3f_t::print(rot.forward());
    // std::cin.get();

    return 0;
}

/*******************************************************************************************
*
*   raylib [models] example - Models loading
*
*   raylib supports multiple models file formats:
*
*     - OBJ  > Text file format. Must include vertex position-texcoords-normals information,
*              if files references some .mtl materials file, it will be loaded (or try to).
*     - GLTF > Text/binary file format. Includes lot of information and it could
*              also reference external files, raylib will try loading mesh and materials data.
*     - IQM  > Binary file format. Includes mesh vertex data but also animation data,
*              raylib can load .iqm animations.
*     - VOX  > Binary file format. MagikaVoxel mesh format:
*              https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
*
*   This example has been created using raylib 4.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014-2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
// #include <raylib/raylib.h>
/*
struct Transform_t {
    float position[3];
    float rotation[4];
    float scale[3];
};

struct Position_t {
    float p {};
};

void test_raylib() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - models loading");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = { 50.0f, 50.0f, 50.0f }; // Camera position
    camera.target = { 0.0f, 10.0f, 0.0f };     // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    Model model = LoadModel("assets/untitled.obj");                 // Load model
    // Model model = LoadModel("assets/untitled.obj");                 // Load model
    // Texture2D texture = LoadTexture("resources/models/obj/castle_diffuse.png"); // Load model texture
    // model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;            // Set map diffuse texture

    Vector3 position = { 0.0f, 0.0f, 0.0f };                    // Set model position

    BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);   // Set model bounds

    // NOTE: bounds are calculated from the original size of the model,
    // if model is scaled on drawing, bounds must be also scaled

    SetCameraMode(camera, CAMERA_FREE);     // Set a free camera mode

    bool selected = false;          // Selected object flag

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);

        // Load new models/textures on drag&drop
        // if (IsFileDropped())
        // {
            
        //     FilePathList droppedFiles = LoadDroppedFiles();

        //     if (droppedFiles.count == 1) // Only support one file dropped
        //     {
        //         if (IsFileExtension(droppedFiles.paths[0], ".obj") ||
        //             IsFileExtension(droppedFiles.paths[0], ".gltf") ||
        //             IsFileExtension(droppedFiles.paths[0], ".glb") ||
        //             IsFileExtension(droppedFiles.paths[0], ".vox") ||
        //             IsFileExtension(droppedFiles.paths[0], ".iqm"))       // Model file formats supported
        //         {
        //             UnloadModel(model);                         // Unload previous model
        //             model = LoadModel(droppedFiles.paths[0]);   // Load new model
        //             model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set current map diffuse texture

        //             bounds = GetMeshBoundingBox(model.meshes[0]);

        //             // TODO: Move camera position from target enough distance to visualize model properly
        //         }
        //         else if (IsFileExtension(droppedFiles.paths[0], ".png"))  // Texture file formats supported
        //         {
        //             // Unload current model texture and load new one
        //             UnloadTexture(texture);
        //             texture = LoadTexture(droppedFiles.paths[0]);
        //             model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        //         }
        //     }

        //     UnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        // }

        // Select model on mouse click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // Check collision between ray and box
            if (GetRayCollisionBox(GetMouseRay(GetMousePosition(), camera), bounds).hit) selected = !selected;
            else selected = false;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawModel(model, position, 12.0f, WHITE);        // Draw 3d model with texture

                DrawGrid(20, 10.0f);         // Draw a grid

                if (selected) DrawBoundingBox(bounds, GREEN);   // Draw selection box

            EndMode3D();

            DrawText("Drag & drop model to load mesh/texture.", 10, GetScreenHeight() - 20, 10, DARKGRAY);
            if (selected) DrawText("MODEL SELECTED", GetScreenWidth() - 110, 10, 10, GREEN);

            DrawText("(c) Castle 3D model by Alberto Cano", screenWidth - 200, screenHeight - 20, 10, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // UnloadTexture(texture);     // Unload texture
    UnloadModel(model);         // Unload model

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

void test_ecs() { 
    ECS::EntityManager_t EntMan {};

    ECS::Entityid_t entity = EntMan.createEntity();
    EntMan.addComponent<Transform_t>(entity);
    EntMan.addComponent<Position_t>(entity);

    // auto update_one = [] (auto, Transform_t& transform) {
    //     std::cout << "I got transform\n";
    // };

    auto update_two = [] (auto, Transform_t& transform, Position_t& pos) {
        std::cout << "I got t+pos\n";
    };

    EntMan.forAllMatching<Transform_t, Position_t>(update_two);

    std::cin.get();
}
*/
    // test_ecs();
    // test_raylib();
    // std::cin.get();

