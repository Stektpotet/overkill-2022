#include <chrono>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <SFML/Audio/SoundBuffer.hpp>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <utilities/timeutils.h>
#include <utilities/mesh.h>
#include <utilities/shapes.h>
#include <utilities/glutils.h>
#include <SFML/Audio/Sound.hpp>
#include <fmt/format.h>
#include "gamelogic.h"
#include "sceneGraph.hpp"


#include "utilities/imageLoader.hpp"
#include "utilities/glfont.h"
#include "utilities/lights.h"

enum KeyFrameAction {
    BOTTOM, TOP
};

#include <timestamps.h>
#include <utilities/imageLoader.hpp>
#include <overkill/graphics_internal/ShaderIntrospector.hpp>
#include <overkill/graphics_internal/FrameBuffer.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/RenderSystem.hpp>
#include <overkill/scene/Scene.hpp>
#include <overkill/scene/Components/Transform.hpp>
#include <overkill/scene/Components/SimpleMeshRendered.hpp>
#include <overkill/scene/Components/Rotator.hpp>
#include <overkill/scene/Input.hpp>
#include <overkill/io.hpp>

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION 
#endif
#include <tiny_obj_loader.h>
#include <overkill/scene/MeshUtility.hpp>
#include <overkill/TextureSystem.hpp>
#include <program.hpp>
#include <overkill/scene/Components/CloudController.hpp>

unsigned int currentKeyFrame = 0;
unsigned int previousKeyFrame = 0;

SceneNode* rootNode;
SceneNode* boxNode;
SceneNode* ballNode;
SceneNode* padNode;
SceneNode* textNode;

OK::UniformBuffer* light_buffer;
OK::UniformBuffer* matrix_buffer;

OK::RenderSystem renderSystem;
OK::Scene* scene;


const int NUM_LIGHTS = 3;
#define MAX_LIGHTS 16 // FOR NOW WE LEAVE IT AS A DEFINE. IDEALLY, WE INJECT THIS DEFINE INTO THE SHADERS UPON READING THEM IN

// These are heap allocated, because they should not be initialised at the start of the program
sf::SoundBuffer* buffer;

sf::Sound* sound;
OK::Texture2D charmapAtlasTexture;
OK::Texture2D brickwall[3];

//const glm::vec3 boxDimensions(180, 90, 90);
//const glm::vec3 padDimensions(30, 3, 40);
//
//glm::vec3 ballPosition(0, ballRadius + padDimensions.y, boxDimensions.z / 2);
//glm::vec3 ballDirection(1, 1, 0.2f);

CommandLineOptions options;

bool hasStarted        = false;
bool hasLost           = false;
bool jumpedToNextFrame = false;
bool isPaused          = false;

bool mouseLeftPressed   = false;
bool mouseLeftReleased  = false;
bool mouseRightPressed  = false;
bool mouseRightReleased = false;

// Modify if you want the music to start further on in the track. Measured in seconds.
const float debug_startTime = 0;
double totalElapsedTime = debug_startTime;
double gameElapsedTime = debug_startTime;

double mouseSensitivity = 1.0;
double lastMouseX = WINDOW_WIDTH / 2;
double lastMouseY = WINDOW_HEIGHT / 2;

struct compute_debug_t {
    //glm::ivec4 neighbourhood[27];
    //glm::ivec4 neighbourhood_scaled[27];
    int neighbourhood_idx[27];
    int pad;

    glm::vec4 relation[27];
    glm::vec4 closest;
    glm::vec4 virtual_points[27];
    glm::vec4 voronoi[27];
    glm::vec4 cell_pos;
};


void initGame(GLFWwindow* window, CommandLineOptions gameOptions) {
    buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile("../res/Hall of the Mountain King.ogg")) {
        return;
    }

    options = gameOptions;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1);

    // ========================== Input "System" setup ==========================

    glfwSetKeyCallback(window, OK::Input::OnInputKey);
    glfwSetCursorPosCallback(window, OK::Input::OnCursorHover);
    glfwSetScrollCallback(window, OK::Input::OnScrollChange);
    glfwSetMouseButtonCallback(window, OK::Input::OnMouseClick);


    // ========================== Shader System setup ==========================

    OK::ShaderSystem::add_shader_sources("../res/shaders");
    OK::ShaderSystem::add_shader_sources("../res/compute_shaders");

    OK::ShaderSystem::push("skybox", { {GL_VERTEX_SHADER, "skybox"}, {GL_FRAGMENT_SHADER, "skybox"} });

    //TODO: have the defualt shaders be pushed into the system by default
    OK::ShaderSystem::push(OK::ShaderSystem::DEFAULT, { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "simple"} });
    OK::ShaderSystem::push(OK::ShaderSystem::VIEWPORT_DEFAULT, { {GL_VERTEX_SHADER, "viewport"}, {GL_FRAGMENT_SHADER, "viewport"} });
    OK::ShaderSystem::push("vignette", { {GL_VERTEX_SHADER, "viewport"}, {GL_FRAGMENT_SHADER, "vignette"} });
    OK::ShaderSystem::push("clouds", { {GL_VERTEX_SHADER, "viewport"}, {GL_FRAGMENT_SHADER, "clouds"} });
    OK::ShaderSystem::push("UI", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "text"} });
    OK::ShaderSystem::push("terrain", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "terrain"} });
    OK::ShaderSystem::push("walls", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "textured"} });
    OK::ShaderSystem::push("skybox", { {GL_VERTEX_SHADER, "skybox"}, {GL_FRAGMENT_SHADER, "skybox"} });
    auto worley = OK::ShaderSystem::push_compute("worley");
    auto worley3d = OK::ShaderSystem::push_compute("worley3d");


    // ======== Uniform Buffer setup ========
    OK::BlockLayout lightBufferLayout("OK_Lights");
    lightBufferLayout.pushBlock(OK::BlockLayout("sun", { { "direction", 16 }, { "intensities", 16 } }));
    lightBufferLayout.pushBlockArray(
        OK::BlockLayout("light", {
            {"position",    16},
            {"intensities", 16},
            {"constant",    4},
            {"linear",      4},
            {"quadratic",   4},
            {"alignment",   4},
            }
        ),
        MAX_LIGHTS
    );
    OK::BlockLayout matrices("OK_Commons", {
        {"projection", 64},
        {"view", 64},
        {"view_projection", 64},
        {"projection_inv", 64},
        {"view_inv", 64},
        {"projection_inv", 64},
        {"view_projection_inv", 64},
        {"projection_params", 16}, // x: near plane, y: far plane
        {"zbuffer_params", 16}, // x: near plane, y: far plane
        {"camera_position", 16},
        {"camera_direction", 16},
        {"time", 16},
    });



    light_buffer = OK::ShaderSystem::makeUniformBuffer<GL_DYNAMIC_DRAW>(lightBufferLayout);
    matrix_buffer = OK::ShaderSystem::makeUniformBuffer<GL_DYNAMIC_DRAW>(matrices);
    auto cloud_buffer = OK::ShaderSystem::makeUniformBuffer<GL_STATIC_DRAW>("OK_Clouds", { { "bounds_min", 16 }, { "bounds_max", 16 } });
    OK::ShaderSystem::bindUniformBlocks();

    // ================= Texture system setup & Frame buffer ==================

    OK::TextureSystem::load("../res/textures/");
    OK::FrameBuffer* mainFrameBuffer = new OK::FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, { GL_NEAREST, GL_CLAMP_TO_EDGE });
    OK::TextureSystem::push2D("__RTEX_COLOR__", mainFrameBuffer->attachRenderTexture());
    OK::TextureSystem::push2D("__RTEX_DEPTH__", mainFrameBuffer->attachDepthTexture());
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "WARN: Framebuffer was not completed!" << std::endl;
    mainFrameBuffer->unbind();


    // =================  3D Worley Noise Generation  ==================

    constexpr glm::ivec3 tex_res = { 256, 256, 256 };
    auto worley_texture = OK::Texture3D(tex_res.x, tex_res.y, tex_res.z, 1);
    OK::TextureSystem::push3D("worley", worley_texture);    
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, tex_res.x, tex_res.y, tex_res.z, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(2, worley_texture.getID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    
    // Generate 1000 points inside of the 3d volume
    constexpr glm::ivec3 cells_per_axis = { 16, 16, 16 };
    constexpr size_t num_rand_points = static_cast<size_t>(cells_per_axis.x) * cells_per_axis.y * cells_per_axis.z;
    glm::vec4* rand_points = static_cast<glm::vec4*>(calloc(num_rand_points, sizeof(glm::vec4)));

    glm::vec3 cell_size = glm::vec3(tex_res) / glm::vec3(cells_per_axis);

    for (size_t z = 0; z < cells_per_axis.z; z++)
    {
        for (size_t y = 0; y < cells_per_axis.y; y++)
        {
            for (size_t x = 0; x < cells_per_axis.x; x++)
            {
                // rand points in cell space (c_uvw)
                // slices = cells_per_axis.x * cells_per_axis.y;
                // rows = cells_per_axis.x;

                rand_points[x + y * cells_per_axis.x + z * cells_per_axis.x * cells_per_axis.y] 
                    = glm::vec4(glm::linearRand(glm::vec3(0), glm::vec3(1)), 0);
            }
        }
    }

    // Transfer the point data through shader storage
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * num_rand_points, rand_points, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); //set binding point to 0
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); //unbind
    //GLuint debug_buffer;
    //// Buffer with one integer slot per kernel for debugging purposes
    //glGenBuffers(1, &debug_buffer);
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, debug_buffer);
    //glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(compute_debug_t) * tex_res.x * tex_res.y * tex_res.z, nullptr, GL_STATIC_READ);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, debug_buffer); //set binding point to 0
    ////glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); //unbind


    worley3d->bind();
    glUniform3i(worley3d->getUniformLocation("resolution"), tex_res.x, tex_res.y, tex_res.z);
    glUniform3i(worley3d->getUniformLocation("cells_per_axis"), cells_per_axis.x, cells_per_axis.y, cells_per_axis.z);
    glDispatchCompute((GLuint)tex_res.x, (GLuint)tex_res.y, (GLuint)tex_res.z);
    glBindTextureUnit(0, worley_texture.getID());
    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    free(rand_points);

    // ========================== Scene construction ==========================

    scene = new OK::Scene("Test Scene");
    scene->main_framebuffer = mainFrameBuffer;
    
    scene->set_viewport_shader(OK::ShaderSystem::get("clouds")); 
    //scene->active_camera()->transform()->position = { 0, 20, -100 };

    //auto game_object =  scene->add_game_object("helloworld");

    auto sun_object = scene->add_game_object("sun");
    sun_object->add_component<OK::Rotator>();
    sun_object->add_component<OK::LightSource>()->light_type = OK::LightSource::Type::DIRECTIONAL;
    

    auto cube = scene->add_game_object("cube");
    cube->get_transform()->set_parent(sun_object->get_transform());
    auto renderer = cube->add_component<OK::SimpleMeshRenderer>();

    //TODO: clean up model and mesh composition and implement multi-mesh and static batching
    OK::Mesh cube_mesh = OK::make_cube(glm::vec3(1, 1, 1));
    OK::Model* model = new OK::Model();
    model->vao.bind();
    model->renderables.emplace_back(OK::Renderable{ cube_mesh, OK::ShaderSystem::get("walls") });
    model->vbo = OK::VertexBuffer(cube_mesh.size(), cube_mesh.vertices.data());
    cube_mesh.vertex_layout.applyToBuffer(model->vbo);
    renderer->model = model;


    auto terrain_renderer = scene->add_game_object("ground")->add_component<OK::SimpleMeshRenderer>();
    
    auto heightmap = OK::IO::read_raw_texture<std::uint16_t>("../res/trondheim.png");
    auto terrain_mesh = OK::make_terrain(heightmap, glm::vec3(heightmap.width / 2, 40, heightmap.height / 2), glm::vec3(-heightmap.width / 4,0, -heightmap.height / 4));
    OK::Model* terrain_model = new OK::Model();
    terrain_model->vao.bind();
    terrain_model->renderables.emplace_back(terrain_mesh, OK::ShaderSystem::get("terrain"));
    terrain_model->vbo = OK::VertexBuffer(terrain_mesh.size(), terrain_mesh.vertices.data());
    terrain_mesh.vertex_layout.applyToBuffer(terrain_model->vbo);
    terrain_renderer->model = terrain_model;

    auto skybox_object = scene->add_game_object("skybox");
    auto skybox_renderer = skybox_object->add_component<OK::SimpleMeshRenderer>();

    OK::Mesh skybox_mesh = OK::make_cube(glm::vec3(heightmap.width / 2, 201, heightmap.height / 2), glm::vec3(0,100,0), true);
    OK::Model* skybox_model = new OK::Model();
    skybox_model->vao.bind();
    skybox_model->renderables.emplace_back(OK::Renderable{ skybox_mesh, OK::ShaderSystem::get("skybox") });
    skybox_model->vbo = OK::VertexBuffer(skybox_mesh.size(), skybox_mesh.vertices.data());
    skybox_mesh.vertex_layout.applyToBuffer(skybox_model->vbo);
    skybox_renderer->model = skybox_model;

    auto v = scene->add_game_object("cloud_controller")->add_component<OK::CloudController>(cloud_buffer, glm::vec3{ -heightmap.width / 4, 40 ,-heightmap.height / 4 }, glm::vec3{ heightmap.width / 4, 200, heightmap.height / 4 });
    auto mr = v->game_object->add_component<OK::SimpleMeshRenderer>();
    mr->model = model;
    v->transform()->position = (glm::vec3{ 22, 9, 22 } *0.5f);
    v->transform()->scale = (glm::vec3{ 18 , 18, 18 });


    scene->propagate_scene_graph();
    OK::TextureSystem::get2D("sun_light_ramp").setSamplingOptions({ GL_LINEAR, GL_CLAMP_TO_EDGE });
    OK::TextureSystem::get2D("sun_light_ramp").bind(8);
    OK::TextureSystem::get2D("terrain_color_ramp").setSamplingOptions({ GL_LINEAR, GL_CLAMP_TO_EDGE });
    OK::TextureSystem::get2D("terrain_color_ramp").bind(9);
    OK::TextureSystem::get2D("Brick03_col").bind(10);
    OK::TextureSystem::get2D("Brick03_nrm").bind(11);
    OK::TextureSystem::get2D("Brick03_rgh").bind(12);

    scene->print_scene_graph();

    getTimeDeltaSeconds();
    getTimeSeconds();
}

void run_gameloop(GLFWwindow* window) {

    double timeDelta = getTimeDeltaSeconds();
    scene->update(timeDelta);
    scene->propagate_scene_graph(); // TODO: dirty-flag transforms? -> only need to update hierarchy on changes
    scene->render(timeDelta);
    //scene->late_update(timeDelta);

    // Utility for grabbing a screenshot for the various tasks
    if (glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS) {
        OK::RawTexture imgGrab = OK::RawTexture(WINDOW_WIDTH, WINDOW_HEIGHT, 4u);
        glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &imgGrab.pixels[0]);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cout << "Encountered error with glReadPixels: " << err << std::endl;
        savePNGFile("screenshot.png", imgGrab);
    }
}
