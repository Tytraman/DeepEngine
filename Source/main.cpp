#include <stdio.h>
#include <math.h>
#include <string>

#include <DE/core/core.hpp>
#include <DE/core/window.hpp>
#include <DE/os/key.hpp>
#include <DE/maths/vec.hpp>
#include <DE/maths/mat.hpp>
#include <DE/ecs/scene.hpp>
#include <DE/graphics/graphic.hpp>
#include <DE/image/png.hpp>
#include <DE/image/bmp.hpp>
#include <DE/core/resources.hpp>
#include <DE/core/memory.hpp>
#include <DE/core/string.hpp>
#include <DE/file/file_object.hpp>
#include <DE/core/settings.hpp>

#include <DE/ecs/entity.hpp>
#include <DE/ecs/component.hpp>

#include <DE/gui/deimgui.hpp>

#include <DE/drivers/opengl/core.hpp>
#include <DE/drivers/opengl/shader.hpp>
#include <DE/drivers/opengl/uniform.hpp>

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	480
#define TARGET_MS		16
#define TARGET_FPS		300

#define SCROLL_SCALE_FACTOR_UP   1.10f
#define SCROLL_SCALE_FACTOR_DOWN 0.90f
#define SCROLL_ROTATE_FACTOR 3.0f

#define PLAYER_SPAWN_X 200.0f
#define PLAYER_SPAWN_Y 200.0f
#define PLAYER_GRAVITY 0.30f

#define RECT1_SPAWN_X  400.0f
#define RECT1_SPAWN_Y  200.0f

void event_callback(deep::window &window, deep::devent e)
{
    switch(e->getType())
    {
        default: break;
        case deep::event_type::MouseMotion:
        {
            if(!window.is_showing_debug_panel())
            {
                deep::scene_id sceneID = deep::scene::get_active_scene_id();
                if(sceneID == deep::badID)
                    break;

                deep::scene *scene = deep::scene::get_scene(sceneID);
                if(scene == nullptr)
                    break;

                deep::camera &camera = deep::scene::get_scene(deep::scene::get_active_scene_id())->get_attached_camera();
                float yaw   = camera.get_yaw();
                float pitch = camera.get_pitch();

                float sensitivity = 0.1f;

                int x = e->getMouseX();
                int y = e->getMouseY();

                int xDiff = x - window.get_width() / 2;
                int yDiff = y - window.get_height() / 2;

                yaw += xDiff * sensitivity;
                pitch -= yDiff * sensitivity;

                if(pitch > 89.0f)
                    pitch = 89.0f;
                if(pitch < -89.0f)
                    pitch = -89.0f;

                camera.set_yaw(yaw);
                camera.set_pitch(pitch);

                window.set_cursor_position(window.get_width() / 2, window.get_height() / 2);

                camera.update_angle_of_view();
            }
        };
    }

    // Appelle le callback par défaut
    deep::window::default_input_callback(window, e);
}

// Fonction appelée en boucle dans la boucle du jeu,
// pour mettre à jour toutes les valeurs du jeu
void update_callback(deep::window & /* win */)
{
    deep::scene_id sceneID = deep::scene::get_active_scene_id();
    deep::entity_collection_id collectionID;
    deep::scene *scene;

    if(sceneID == deep::badID)
        return;

    scene = deep::scene::get_scene(sceneID);
    if(scene == nullptr)
        return;

    collectionID = deep::scene::get_entity_collection(sceneID);

    float cameraSpeed = 0.15f;

    deep::camera &camera = scene->get_attached_camera();
    deep::vec3<float> cameraPos = camera.get_position();
    deep::vec3<float> cameraFront = camera.get_front();
    deep::vec3<float> cameraRight = deep::vec3<float>::normalize(deep::vec3<float>::cross(cameraFront, camera.get_up()));

    deep::entity_manager::entity entity = deep::entity_manager::entity::bad();

    // Augmentation de la vitesse de déplacement de la caméra.
    if(deep::key::isPressed(deep::dkey::LShift))
    {
        cameraSpeed += 0.10f;
    }

    // Déplacer la caméra vers l'avant.
    if(deep::key::isPressed(deep::dkey::Z))
    {
        camera.move_forward(cameraSpeed);
    }

    // Déplacer la caméra vers la gauche.
    if(deep::key::isPressed(deep::dkey::Q))
    {
        camera.move_left(cameraSpeed);
    }

    // Déplacer la caméra vers la droite.
    if(deep::key::isPressed(deep::dkey::D))
    {
        camera.move_right(cameraSpeed);
    }

    // Déplacer la caméra vers l'arrière.
    if(deep::key::isPressed(deep::dkey::S))
    {
        camera.move_backward(cameraSpeed);
    }

    // Fait monter la caméra.
    if(deep::key::isPressed(deep::dkey::Space))
    {
        camera.move_up(cameraSpeed);
    }

    // Fait descendre la caméra.
    if(deep::key::isPressed(deep::dkey::LCtrl))
    {
        camera.move_down(cameraSpeed);
    }

    camera.update_angle_of_view();
}

#include <iostream>

#undef main
int main()
{
    switch(deep::core::init("DeepEngine [" DE_VERSION "]", 0, 0, 0))
    {
        default:
            break;
        case deep::core_init_status::CannotInstantiateObjects:
        {
            fprintf(stderr, "Cannot instanciate objects.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::InstanceAlreadyExists:
        {
            fprintf(stderr, "Process already running.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::NoEnoughDiskSpace:
        {
            fprintf(stderr, "No enough disk space.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::NoEnoughMemory:
        {
            fprintf(stderr, "No enough memory.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotInitCOM:
        {
            fprintf(stderr, "Cannot initialize COM.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotQueryCpuInfo:
        {
            fprintf(stderr, "Cannot query CPU info.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotInitNtDll:
        {
            fprintf(stderr, "Cannot load NTDLL.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotLoadStdStream:
        {
            fprintf(stderr, "Cannot load STD stream.\n");
        } return EXIT_FAILURE;
        case deep::core_init_status::CannotLoadEngineSettings:
        {
            fprintf(stderr, "Cannot load engine settings.\n");
        } return EXIT_FAILURE;
    }

    deep::core::out() << "pwd: " << deep::core::get_pwd().str() << "\n";

    deep::window win(TARGET_MS, TARGET_FPS);
    win.set_event_callback(event_callback);
    win.set_update_callback(update_callback);

    if(!deep::window::create(win, "DeepEngine [" DE_VERSION "]", deep::size(WINDOW_WIDTH, WINDOW_HEIGHT)))
    {
        deep::core::shutdown();

        return EXIT_FAILURE;
    }

    deep::engine_settings *engineSettings = deep::engine_settings::get_singleton();
    deep::resource_manager *resourcesManager = deep::resource_manager::get_singleton();
    deep::GL3::texture_manager *textureManager = deep::GL3::texture_manager::get_singleton();

    // Charge les ressources nécessaires au jeu.
    if(!resourcesManager->init(engineSettings->get_resources_directory().str()))
    {
        deep::core::shutdown();
        return 1;
    }

    deep::im_gui_window::init(win);

    deep::scene_id sceneID = deep::scene::create_scene("scn_main");

    deep::entity_collection_id collectionID = deep::scene::get_entity_collection(sceneID);

    deep::core::out() <<
        DE_TERM_FG_YELLOW "====================[ " DE_TERM_FG_RED "\\OpenGL/" DE_TERM_FG_YELLOW " ]====================\n" DE_TERM_RESET
        "Version: "                  << deep::GL3::core::query_version() << "\n"
        "Max vertex attribs: "       << deep::GL3::core::query_max_vertex_attribs() << "\n"
        "Max texture image units: "  << deep::GL3::core::query_max_texture_image_units() << "\n"
        "Max texture size: "         << deep::GL3::core::query_max_texture_size() << "\n"
        "Max 3D texture size: "      << deep::GL3::core::query_max_3D_texture_size() << "\n"
        "Max array texture layers: " << deep::GL3::core::query_max_array_texture_layers() << "\n"
        DE_TERM_FG_YELLOW "====================================================\n" DE_TERM_RESET;

    deep::bmp mcGrass, mcGrassTop, mcDirt;
    if(!resourcesManager->load_bmp("grass_block_side.bmp", mcGrass))
    {
        deep::core::err() << "Unable to load resource:" << GetLastError() << "\n";
        return EXIT_FAILURE;
    }

    if(!resourcesManager->load_bmp("grass_block_top.bmp", mcGrassTop))
    {
        deep::core::err() << "Unable to load resource.\n";
        return EXIT_FAILURE;
    }

    if(!resourcesManager->load_bmp("dirt.bmp", mcDirt))
    {
        deep::core::err() << "Unable to load resource.\n";
        return EXIT_FAILURE;
    }

    if(!mcGrass.add(mcGrassTop))
    {
        deep::core::err() << "Unable to add bmp image.\n";
        return EXIT_FAILURE;
    }

    if(!mcGrass.add(mcDirt, deep::vec2<int32_t>(0, mcGrass.get_height())))
    {
        deep::core::err() << "Unable to add bmp image.\n";
        return EXIT_FAILURE;
    }

    deep::GL3::gl_id mcTexture = textureManager->create_2D("mc_texture");
    textureManager->bind(mcTexture, 0);
    textureManager->set_texture_wrapping_s(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_wrapping_t(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_filtering(deep::GL3::texture_manager::gl_texture_filter::Nearest);
    textureManager->transmit_texture(mcGrass.get_image(), mcGrass.get_width(), mcGrass.get_height(), mcGrass.get_color_type());

    deep::bmp skyboxLeft;
    skyboxLeft.create_from_file("..\\resources\\textures\\skybox_left.bmp");
    skyboxLeft.vertical_flip();
    skyboxLeft.horizontal_flip();

    deep::bmp skyboxFront;
    skyboxFront.create_from_file("..\\resources\\textures\\skybox_front.bmp");
    skyboxFront.vertical_flip();
    skyboxFront.horizontal_flip();

    deep::bmp skyboxRight;
    skyboxRight.create_from_file("..\\resources\\textures\\skybox_right.bmp");
    skyboxRight.vertical_flip();
    skyboxRight.horizontal_flip();

    deep::bmp skyboxBack;
    skyboxBack.create_from_file("..\\resources\\textures\\skybox_back.bmp");
    skyboxBack.vertical_flip();
    skyboxBack.horizontal_flip();

    deep::bmp skyboxBottom;
    skyboxBottom.create_from_file("..\\resources\\textures\\skybox_bottom.bmp");

    deep::bmp skyboxTop;
    skyboxTop.create_from_file("..\\resources\\textures\\skybox_top.bmp");

    deep::vec4<float> black(0.0f, 0.0f, 0.0f, 1.0);
    deep::vec4<float> lightCubeColor(1.0f, 1.0f, 1.0f, 1.0);
    deep::vec4<float> lightedCubeColor(1.0f, 0.5f, 0.31f, 1.0f);

    deep::GL3::gl_id skybox = textureManager->create_2D("skybox", deep::GL3::texture_manager::gl_texture_type::texture_cubemap);
    textureManager->bind(skybox);
    textureManager->set_texture_wrapping_s_cubemaps(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_wrapping_t_cubemaps(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_wrapping_r_cubemaps(deep::GL3::texture_manager::gl_texture_wrap::ClampToEdge);
    textureManager->set_texture_filtering_cubemaps(deep::GL3::texture_manager::gl_texture_filter::Linear);
    textureManager->transmit_texture_cubemaps(skyboxLeft.get_image(), skyboxFront.get_image(), skyboxRight.get_image(), skyboxBack.get_image(), skyboxBottom.get_image(), skyboxTop.get_image(), skyboxTop.get_width(), skyboxTop.get_height(), skyboxTop.get_color_type());

    deep::GL3::program_manager *programManager = deep::GL3::program_manager::get_singleton();

    deep::hash_function hash = programManager->get_hash_function();
    deep::GL3::gl_id defaultProgram = hash("default");
    deep::GL3::gl_id skyboxProgram  = hash("skybox");
    deep::GL3::gl_id postProcessProgram = hash("post_processing");
    deep::GL3::gl_id lightedObjectProgram = hash("lighted_colored_object");
    deep::GL3::gl_id lightSourceObjectProgram = hash("light_source_object");
    deep::GL3::gl_id lightedTexturedObjectProgram = hash("lighted_textured_object");

    programManager->use(defaultProgram);

    int mTrs = deep::GL3::uniform_manager::find(defaultProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    int mRotX = deep::GL3::uniform_manager::find(defaultProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    int mRotY = deep::GL3::uniform_manager::find(defaultProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    int mRotZ = deep::GL3::uniform_manager::find(defaultProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    int mScl = deep::GL3::uniform_manager::find(defaultProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    int view = deep::GL3::uniform_manager::find(defaultProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::fmat4x4(0.0f));

    int proj = deep::GL3::uniform_manager::find(defaultProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::fmat4x4(0.0f));

    int myTex = deep::GL3::uniform_manager::find(defaultProgram, "myTex");
    if(myTex != -1)
        programManager->add_uniform("myTex", myTex, 0);

    programManager->use(skyboxProgram);

    int skyboxView = deep::GL3::uniform_manager::find(skyboxProgram, "view");
    if(skyboxView != -1)
        programManager->add_uniform("view", skyboxView, deep::fmat4x4(0.0f));

    int skyboxProj = deep::GL3::uniform_manager::find(skyboxProgram, "proj");
    if(skyboxProj != -1)
        programManager->add_uniform("proj", skyboxProj, deep::fmat4x4(0.0f));

    int skyboxCube = deep::GL3::uniform_manager::find(skyboxProgram, "skybox");
    if(skyboxCube != -1)
        programManager->add_uniform("skybox", skyboxCube, 0);

    // ===== LIGHTED OBJECT ===== //

    deep::vec3<float> lightSourcePos = deep::vec3<float>(-3.5f, 4.0f, -1.0f);

    programManager->use(lightedObjectProgram);
    
    mTrs = deep::GL3::uniform_manager::find(lightedObjectProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    mRotX = deep::GL3::uniform_manager::find(lightedObjectProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    mRotY = deep::GL3::uniform_manager::find(lightedObjectProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    mRotZ = deep::GL3::uniform_manager::find(lightedObjectProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    mScl = deep::GL3::uniform_manager::find(lightedObjectProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    view = deep::GL3::uniform_manager::find(lightedObjectProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::fmat4x4(0.0f));

    proj = deep::GL3::uniform_manager::find(lightedObjectProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::fmat4x4(0.0f));

    int objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.ambient");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.ambient", objectColorLoc, deep::vec3<float>(1.0f, 0.5f, 0.31f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.diffuse", objectColorLoc, deep::vec3<float>(1.0f, 0.5f, 0.31f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.specular", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deMaterial.shininess");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.shininess", objectColorLoc, 32.0f);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.position");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.position", objectColorLoc, lightSourcePos);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.ambient");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.ambient", objectColorLoc, deep::vec3<float>(0.2f, 0.2f, 0.2f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.diffuse", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deLight.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.specular", objectColorLoc, lightCubeColor.rgb());

    objectColorLoc = deep::GL3::uniform_manager::find(lightedObjectProgram, "deViewPos");
    if(objectColorLoc != -1)
        programManager->add_uniform("deViewPos", objectColorLoc, deep::vec3<float>());

    // ===========================================

    programManager->use(lightedTexturedObjectProgram);
    
    mTrs = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    mRotX = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    mRotY = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    mRotZ = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    mScl = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    view = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::fmat4x4(0.0f));

    proj = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::fmat4x4(0.0f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deMaterial.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.diffuse", objectColorLoc, 0);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deMaterial.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.specular", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deMaterial.shininess");
    if(objectColorLoc != -1)
        programManager->add_uniform("deMaterial.shininess", objectColorLoc, 32.0f);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.position");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.position", objectColorLoc, lightSourcePos);

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.ambient");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.ambient", objectColorLoc, deep::vec3<float>(0.2f, 0.2f, 0.2f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.diffuse");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.diffuse", objectColorLoc, deep::vec3<float>(0.5f, 0.5f, 0.5f));

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deLight.specular");
    if(objectColorLoc != -1)
        programManager->add_uniform("deLight.specular", objectColorLoc, lightCubeColor.rgb());

    objectColorLoc = deep::GL3::uniform_manager::find(lightedTexturedObjectProgram, "deViewPos");
    if(objectColorLoc != -1)
        programManager->add_uniform("deViewPos", objectColorLoc, deep::vec3<float>());

    // ===========================================


    programManager->use(lightSourceObjectProgram);

    mTrs = deep::GL3::uniform_manager::find(defaultProgram, "mTrs");
    if(mTrs != -1)
        programManager->add_uniform("mTrs", mTrs, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    mRotX = deep::GL3::uniform_manager::find(defaultProgram, "mRotX");
    if(mRotX != -1)
        programManager->add_uniform("mRotX", mRotX, 0.0f);

    mRotY = deep::GL3::uniform_manager::find(defaultProgram, "mRotY");
    if(mRotY != -1)
        programManager->add_uniform("mRotY", mRotY, 0.0f);

    mRotZ = deep::GL3::uniform_manager::find(defaultProgram, "mRotZ");
    if(mRotZ != -1)
        programManager->add_uniform("mRotZ", mRotZ, 0.0f);

    mScl = deep::GL3::uniform_manager::find(defaultProgram, "mScl");
    if(mScl != -1)
        programManager->add_uniform("mScl", mScl, deep::vec3<float>(0.0f, 0.0f, 0.0f));

    view = deep::GL3::uniform_manager::find(defaultProgram, "view");
    if(view != -1)
        programManager->add_uniform("view", view, deep::fmat4x4(0.0f));

    proj = deep::GL3::uniform_manager::find(defaultProgram, "proj");
    if(proj != -1)
        programManager->add_uniform("proj", proj, deep::fmat4x4(0.0f));

    programManager->use(postProcessProgram);

    int postScreenTexture = deep::GL3::uniform_manager::find(postProcessProgram, "screenTexture");
    if(postScreenTexture != -1)
        programManager->add_uniform("screenTexture", postScreenTexture, 0);

    deep::entity_manager *entityManager = deep::entity_manager::get_singleton();

    deep::graphic::cube_texture_points points;
    points.frontTopLeft.x     = points.backTopLeft.x     = points.rightTopLeft.x     = points.leftTopLeft.x     = 0.0f;
    points.frontTopLeft.y     = points.backTopLeft.y     = points.rightTopLeft.y     = points.leftTopLeft.y     = 0.5f;
    points.frontTopRight.x    = points.backTopRight.x    = points.rightTopRight.x    = points.leftTopRight.x    = 0.5f;
    points.frontTopRight.y    = points.backTopRight.y    = points.rightTopRight.y    = points.leftTopRight.y    = 0.5f;
    points.frontBottomLeft.x  = points.backBottomLeft.x  = points.rightBottomLeft.x  = points.leftBottomLeft.x  = 0.0f;
    points.frontBottomLeft.y  = points.backBottomLeft.y  = points.rightBottomLeft.y  = points.leftBottomLeft.y  = 0.0f;
    points.frontBottomRight.x = points.backBottomRight.x = points.rightBottomRight.x = points.leftBottomRight.x = 0.5f;
    points.frontBottomRight.y = points.backBottomRight.y = points.rightBottomRight.y = points.leftBottomRight.y = 0.0f;

    points.bottomTopLeft.x     = 0.0f;
    points.bottomTopLeft.y     = 1.0f;
    points.bottomTopRight.x    = 0.5f;
    points.bottomTopRight.y    = 1.0f;
    points.bottomBottomLeft.x  = 0.0f;
    points.bottomBottomLeft.y  = 0.5f;
    points.bottomBottomRight.x = 0.5f;
    points.bottomBottomRight.y = 0.5f;

    points.topTopLeft.x     = 0.5f;
    points.topTopLeft.y     = 0.5f;
    points.topTopRight.x    = 1.0f;
    points.topTopRight.y    = 0.5f;
    points.topBottomLeft.x  = 0.5f;
    points.topBottomLeft.y  = 0.0f;
    points.topBottomRight.x = 1.0f;
    points.topBottomRight.y = 0.0f;

    deep::polygon lightedCube = deep::graphic::create_cube("lighted_cube", lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor);
    deep::polygon testLightedCube = deep::graphic::create_cube("test_lighted_cube", lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, lightedCubeColor, points);
    deep::polygon lightSourceCube = deep::graphic::create_cube("light_source_cube", lightCubeColor, lightCubeColor, lightCubeColor, lightCubeColor, lightCubeColor, lightCubeColor);

    deep::color_material *lightedMaterial1 = deep::mem::alloc_type<deep::color_material>(lightedObjectProgram, deep::vec3<float>(0.75f, 0.32f, 0.67f), deep::vec3<float>(0.75f, 0.32f, 0.67f), deep::vec3<float>(0.5f, 0.5f, 0.5f), 32.0f);
    deep::texture_material *lightedMaterial2 = deep::mem::alloc_type<deep::texture_material>(lightedTexturedObjectProgram, mcTexture, deep::vec3<float>(0.5f, 0.5f, 0.5f), 64.0f);
    deep::color_material *lightSourceMaterial = deep::mem::alloc_type<deep::color_material>(lightSourceObjectProgram, deep::vec3<float>(1.0f, 1.0f, 1.0f), deep::vec3<float>(1.0f, 1.0f, 1.0f), deep::vec3<float>(0.5f, 0.5f, 0.5f), 32.0f);

    entityManager->create_entity("lighted_entity", collectionID, lightedCube, deep::vec3<float>(0.0f, 0.0f, -5.0f), deep::vec3<float>(3.0f, 3.0f, 3.0f), lightedMaterial1);
    entityManager->create_entity("test_lighted_entity", collectionID, testLightedCube, deep::vec3<float>(0.0f, 6.0f, -5.0f), deep::vec3<float>(3.0f, 3.0f, 3.0f), lightedMaterial2);
    entityManager->create_entity("light_source_entity", collectionID, lightSourceCube, lightSourcePos, deep::vec3<float>(1.0f, 1.0f, 1.0f), lightSourceMaterial);

    //== deep::component_id testDrawableComponentID 

    // Affiche la skybox à la fin pour optimiser les appelles au fragment shader = FPS++
    //deep::entity_manager::entity skyboxEnt = deep::graphic::create_cubemap("skybox", skyboxProgram, collectionID, deep::fvec3(0.0f, 0.0f, 0.0f), 5005.0f, 105.0f, 2005.0f, skybox, 0);

    deep::scene::set_active_scene(sceneID);

    win.set_showing_debug_panel(true);

    // Lance la boucle du jeu, bloquant.
    win.run();

    deep::core::out() << DE_TERM_FG_RED "~Good-bye~" DE_TERM_RESET "\n";

    programManager->destroy_all_programs();
    resourcesManager->shutdown();
    deep::core::shutdown();

    win.destroy();

    _CrtDumpMemoryLeaks();

    return EXIT_SUCCESS;
}
