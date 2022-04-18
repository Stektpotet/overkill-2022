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
#include <overkill/io.hpp>

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION 
#endif
#include <tiny_obj_loader.h>
#include <overkill/scene/MeshUtility.hpp>
#include <overkill/TextureSystem.hpp>

double padPositionX = 0;
double padPositionZ = 0;

unsigned int currentKeyFrame = 0;
unsigned int previousKeyFrame = 0;

SceneNode* rootNode;
SceneNode* boxNode;
SceneNode* ballNode;
SceneNode* padNode;
SceneNode* textNode;

OK::UniformBuffer* lightBuffer;
OK::UniformBuffer* matrixBuffer;

OK::RenderSystem renderSystem;
OK::Scene* scene;

double ballRadius = 3.0f;

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
glm::mat4 viewProjection;

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
void mouseCallback(GLFWwindow* window, double x, double y) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    double deltaX = x - lastMouseX;
    double deltaY = y - lastMouseY;

    padPositionX -= mouseSensitivity * deltaX / windowWidth;
    padPositionZ -= mouseSensitivity * deltaY / windowHeight;

    if (padPositionX > 1) padPositionX = 1;
    if (padPositionX < 0) padPositionX = 0;
    if (padPositionZ > 1) padPositionZ = 1;
    if (padPositionZ < 0) padPositionZ = 0;

    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
}

PointLight lights[NUM_LIGHTS];



void initGame(GLFWwindow* window, CommandLineOptions gameOptions) {
    buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile("../res/Hall of the Mountain King.ogg")) {
        return;
    }

    options = gameOptions;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPosCallback(window, mouseCallback);


    OK::TextureSystem::load("../res/textures/");

    OK::ShaderSystem::add_shader_sources("../res/shaders");

    OK::ShaderProgram* default_shader = OK::ShaderSystem::push(OK::ShaderSystem::DEFAULT, { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "simple"} });
    OK::ShaderProgram* default_viewport_shader = OK::ShaderSystem::push(OK::ShaderSystem::VIEWPORT_DEFAULT, { {GL_VERTEX_SHADER, "viewport"}, {GL_FRAGMENT_SHADER, "viewport"} });
    OK::ShaderProgram* uiShader = OK::ShaderSystem::push("UI", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "text"} });
    OK::ShaderProgram* simpleGeometryShader = OK::ShaderSystem::push("simpleGeometry", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "simple"} });
    OK::ShaderProgram* texturedShader = OK::ShaderSystem::push("walls", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "textured"} });

    OK::BlockLayout lightBufferLayout("OK_Lights");
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


    lightBuffer = OK::ShaderSystem::makeUniformBuffer<GL_DYNAMIC_DRAW>(lightBufferLayout);
    matrixBuffer = OK::ShaderSystem::makeUniformBuffer<GL_DYNAMIC_DRAW>("OK_Commons", { { "projection", 64 }, { "view", 64 } });
    OK::ShaderSystem::bindUniformBlocks();


    OK::FrameBuffer* mainFrameBuffer = new OK::FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, { GL_NEAREST, GL_CLAMP_TO_EDGE });
    OK::TextureSystem::push2D("__RTEX_COLOR__", mainFrameBuffer->attachRenderTexture());
    OK::TextureSystem::push2D("__RTEX_DEPTH__", mainFrameBuffer->attachDepthTexture());
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "WARN: Framebuffer was not completed!" << std::endl;
    mainFrameBuffer->unbind();

    scene = new OK::Scene("Test Scene");
    scene->main_framebuffer = mainFrameBuffer;

    auto game_object =  scene->add_game_object("helloworld");

    game_object = scene->add_game_object("hello");
    game_object->add_component<OK::Rotator>();

    auto game_object2 = scene->add_game_object("world");
    game_object2->get_transform()->set_parent(game_object->get_transform());
    auto renderer = game_object2->add_component<OK::SimpleMeshRenderer>();

    scene->active_camera()->transform()->position = {0, 1, 5};
    scene->active_camera()->transform()->rotation = glm::quat(glm::vec3{ glm::radians(-20.0f), 0, 0 });

    auto terrain_renderer = scene->add_game_object("ground")->add_component<OK::SimpleMeshRenderer>();
    
    OK::Mesh cube_mesh = OK::make_cube(glm::vec3(4,2,4), glm::vec3(0), true);
    OK::Model* model = new OK::Model();
    model->vao.bind();
    model->renderables.emplace_back(OK::Renderable{ cube_mesh, OK::ShaderSystem::get("walls") });
    model->vbo = OK::VertexBuffer(cube_mesh.size(), cube_mesh.vertices.data());
    cube_mesh.vertex_layout.applyToBuffer(model->vbo);
    renderer->model = model;
    
    auto terrain_mesh = OK::make_terrain(OK::IO::read_raw_texture<std::uint16_t>("../res/heightmap.png"), glm::vec3(504 / 2, 20, 1004 / 2));
    OK::Model* terrain_model = new OK::Model();
    terrain_model->vao.bind();
    terrain_model->renderables.emplace_back(terrain_mesh, OK::ShaderSystem::get("__DEFAULT__"));
    terrain_model->vbo = OK::VertexBuffer(terrain_mesh.size(), terrain_mesh.vertices.data());
    terrain_mesh.vertex_layout.applyToBuffer(terrain_model->vbo);
    terrain_renderer->model = terrain_model;
    terrain_renderer->transform()->position = glm::vec3(-125, -15, -250);
    terrain_renderer->transform()->scale = glm::vec3(252, 75, 502);

    scene->propagate_scene_graph();

    OK::TextureSystem::get2D("charmap").bind(0);
    OK::TextureSystem::get2D("Brick03_col").bind(1);
    OK::TextureSystem::get2D("Brick03_nrm").bind(2);
    OK::TextureSystem::get2D("Brick03_rgh").bind(3);

    auto& transforms = scene->get_components<OK::Transform>();
    for (const auto& tr : transforms)
    {
        std::cout << tr->game_object->get_name() << "::Transform" << std::endl;
    }

    getTimeDeltaSeconds();

    std::cout << "Ready. Click to start!" << std::endl;
}

void updateFrame(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double timeDelta = getTimeDeltaSeconds();


    const float cameraWallOffset = 30; // Arbitrary addition to prevent ball from going too much into camera


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
        mouseLeftPressed = true;
        mouseLeftReleased = false;
    } else {
        mouseLeftReleased = mouseLeftPressed;
        mouseLeftPressed = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
        mouseRightPressed = true;
        mouseRightReleased = false;
    } else {
        mouseRightReleased = mouseRightPressed;
        mouseRightPressed = false;
    }

    if(!hasStarted) {
        if (mouseLeftPressed) {
            if (options.enableMusic) {
                sound = new sf::Sound();
                sound->setBuffer(*buffer);
                sf::Time startTime = sf::seconds(debug_startTime);
                sound->setPlayingOffset(startTime);
                sound->play();
            }
            totalElapsedTime = debug_startTime;
            gameElapsedTime = debug_startTime;
            hasStarted = true;
        }

    } else {
        totalElapsedTime += timeDelta;
        if(hasLost) {
            if (mouseLeftReleased) {
                hasLost = false;
                hasStarted = false;
                currentKeyFrame = 0;
                previousKeyFrame = 0;
            }
        } else if (isPaused) {
            if (mouseRightReleased) {
                isPaused = false;
                if (options.enableMusic) {
                    sound->play();
                }
            }
        } else {
            gameElapsedTime += timeDelta;
            if (mouseRightReleased) {
                isPaused = true;
                if (options.enableMusic) {
                    sound->pause();
                }
            }
            // Get the timing for the beat of the song
            for (unsigned int i = currentKeyFrame; i < keyFrameTimeStamps.size(); i++) {
                if (gameElapsedTime < keyFrameTimeStamps.at(i)) {
                    continue;
                }
                currentKeyFrame = i;
            }

            jumpedToNextFrame = currentKeyFrame != previousKeyFrame;
            previousKeyFrame = currentKeyFrame;

            double frameStart = keyFrameTimeStamps.at(currentKeyFrame);
            double frameEnd = keyFrameTimeStamps.at(currentKeyFrame + 1); // Assumes last keyframe at infinity

            double elapsedTimeInFrame = gameElapsedTime - frameStart;
            double frameDuration = frameEnd - frameStart;
            double fractionFrameComplete = elapsedTimeInFrame / frameDuration;

            double ballYCoord;

            KeyFrameAction currentOrigin = keyFrameDirections.at(currentKeyFrame);
            KeyFrameAction currentDestination = keyFrameDirections.at(currentKeyFrame + 1);
        }
    }
}

void renderFrame(GLFWwindow* window) {

    double timeDelta = getTimeDeltaSeconds();
    //renderNode(rootNode);
    scene->propagate_scene_graph(); // TODO: dirty-flag transforms? -> only need to update hierarchy on changes
    scene->update(timeDelta);
    scene->render();
    scene->late_update(timeDelta);
    

    // Utility for grabbing a screenshot for the various tasks
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        OK::RawTexture imgGrab = OK::RawTexture(WINDOW_WIDTH, WINDOW_HEIGHT, 4u);
        glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &imgGrab.pixels[0]);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cout << "Encountered error with glReadPixels: " << err << std::endl;
        savePNGFile("screenshot.png", imgGrab);
    }
}
