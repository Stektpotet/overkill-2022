#include <chrono>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <utilities/shader.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <utilities/timeutils.h>
#include <utilities/mesh.h>
#include <utilities/shapes.h>
#include <utilities/glutils.h>
#include <SFML/Audio/Sound.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/format.h>
#include "gamelogic.h"
#include "sceneGraph.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>


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
#include <overkill/io.hpp>
#include <overkill/ecs/ecs.hpp>

#define TINYOBJLOADER_IMPLEMENTATION 
#include <tiny_obj_loader.h>

double padPositionX = 0;
double padPositionZ = 0;

unsigned int currentKeyFrame = 0;
unsigned int previousKeyFrame = 0;

SceneNode* rootNode;
SceneNode* boxNode;
SceneNode* ballNode;
SceneNode* padNode;
SceneNode* textNode;

OK::UniformBuffer lightBuffer;
OK::UniformBuffer matrixBuffer;

OK::ShaderSystem shaderSystem("../res/shaders");
OK::RenderSystem renderSystem;

double ballRadius = 3.0f;

const int NUM_LIGHTS = 3;
#define MAX_LIGHTS 16 // FOR NOW WE LEAVE IT AS A DEFINE. IDEALLY, WE INJECT THIS DEFINE INTO THE SHADERS UPON READING THEM IN

// These are heap allocated, because they should not be initialised at the start of the program
sf::SoundBuffer* buffer;

sf::Sound* sound;
OK::Texture2D charmapAtlasTexture;
OK::Texture2D brickwall[3];

const glm::vec3 boxDimensions(180, 90, 90);
const glm::vec3 padDimensions(30, 3, 40);

glm::vec3 ballPosition(0, ballRadius + padDimensions.y, boxDimensions.z / 2);
glm::vec3 ballDirection(1, 1, 0.2f);
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




    OK::ShaderProgram uiShader = shaderSystem.push("UI", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "text"} });
    OK::ShaderProgram simpleGeometryShader = shaderSystem.push("simpleGeometry", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "simple"} });
    OK::ShaderProgram texturedShader = shaderSystem.push("walls", { {GL_VERTEX_SHADER, "simple"}, {GL_FRAGMENT_SHADER, "textured"} });
    
    auto shaders = { uiShader, simpleGeometryShader, texturedShader };

    OK::BlockLayout lightBufferLayout;
    lightBufferLayout.pushBlockArray(
        OK::BlockLayout("light", {
            {"position",    16},
            {"intensities", 16},
            {"constant",    4},
            {"linear",      4},
            {"quadratic",   4},
            {"alignment",   4},
        }), 
        MAX_LIGHTS
    );

    lightBuffer = shaderSystem.makeUniformBuffer<GL_DYNAMIC_DRAW>("OK_Lights", lightBufferLayout);

    shaderSystem.bindUniformBlocks();

    OK::RawTexture charmap_raw = loadPNGFile("../res/textures/charmap.png");
    OK::RawTexture brickwall_albedo_raw = loadPNGFile("../res/textures/Brick03_col.png");
    OK::RawTexture brickwall_nrm_raw = loadPNGFile("../res/textures/Brick03_nrm.png");
    OK::RawTexture brickwall_spec_raw = loadPNGFile("../res/textures/Brick03_rgh.png");

    charmapAtlasTexture = OK::Texture2D(charmap_raw, { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT });
    brickwall[0] = OK::Texture2D(brickwall_albedo_raw);
    brickwall[1] = OK::Texture2D(brickwall_nrm_raw, { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT } );
    brickwall[2] = OK::Texture2D(brickwall_spec_raw);
    charmapAtlasTexture.bind(0); 
    brickwall[0].bind(1);
    brickwall[1].bind(2);
    brickwall[2].bind(3);

    // Create meshes
    Mesh pad = cube(padDimensions, glm::vec2(30, 40), true);
    Mesh box = cube(boxDimensions, glm::vec2(90), true, true);
    Mesh sphere = generateSphere(1.0, 40, 40);
    std::string text = "The cake is a lie!";
    Mesh textMesh = generateTextGeometryBuffer(text, 32.f / 29.f, 29.f * text.length());
    
    // Fill buffers
    unsigned int ballVAO = generateBuffer(sphere);
    unsigned int boxVAO = generateBuffer(box);
    unsigned int padVAO = generateBuffer(pad);
    unsigned int textVAO = generateBuffer(textMesh);

    // Construct scene
    rootNode = createSceneNode();
    boxNode = createSceneNode(SceneNodeType::NORMAL_MAPPED);
    padNode = createSceneNode(SceneNodeType::GEOMETRY);
    ballNode = createSceneNode(SceneNodeType::GEOMETRY);
    textNode = createSceneNode(SceneNodeType::UI);
    // center the text in the bottom of the view
    textNode->position = { WINDOW_WIDTH * 0.5f - 14.5f * text.length(), 16.f, 0 }; 

    rootNode->children.push_back(boxNode);
    rootNode->children.push_back(padNode);
    rootNode->children.push_back(ballNode);
    rootNode->children.push_back(textNode);

    boxNode->vertexArrayObjectID = boxVAO;
    boxNode->VAOIndexCount = box.indices.size();

    padNode->vertexArrayObjectID = padVAO;
    padNode->VAOIndexCount = pad.indices.size();

    ballNode->vertexArrayObjectID = ballVAO;
    ballNode->VAOIndexCount = sphere.indices.size();

    textNode->vertexArrayObjectID = textVAO;
    textNode->VAOIndexCount = textMesh.indices.size();

    glm::vec3 intensities[NUM_LIGHTS];
    std::fill(std::begin(intensities), std::end(intensities), glm::vec3{1, .8, .5}); // Set the lights to be of a yellow/orange tint

    SceneNode* lightSources[NUM_LIGHTS];
    for (size_t i = 0; i < NUM_LIGHTS; i++)
    {
        lights[i].setRadius(100);
        lights[i].intensities = intensities[i];
        lightSources[i] = createSceneNode(SceneNodeType::POINT_LIGHT);
        lightSources[i]->lightID = i;
        lightSources[i]->position = glm::vec3(i * 20 - (20 * (NUM_LIGHTS - 2)) * 0.5f, 2, 20);
    }
    for (size_t i = 2; i < NUM_LIGHTS; i++)
    {
        padNode->children.push_back(lightSources[i]);
    }
    // Place light sourcces in the upper back corners of the box
    lightSources[0]->position = { -85, 40, -40 };
    lightSources[1]->position = {  85, 40, -40 };
    boxNode->children.push_back(lightSources[0]);
    boxNode->children.push_back(lightSources[1]);

    getTimeDeltaSeconds();

    std::cout << fmt::format("Initialized scene with {} SceneNodes.", totalChildren(rootNode)) << std::endl;

    std::cout << "Ready. Click to start!" << std::endl;
}

void updateFrame(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double timeDelta = getTimeDeltaSeconds();

    const float ballBottomY = boxNode->position.y - (boxDimensions.y/2) + ballRadius + padDimensions.y;
    const float ballTopY    = boxNode->position.y + (boxDimensions.y/2) - ballRadius;
    const float BallVerticalTravelDistance = ballTopY - ballBottomY;

    const float cameraWallOffset = 30; // Arbitrary addition to prevent ball from going too much into camera

    const float ballMinX = boxNode->position.x - (boxDimensions.x/2) + ballRadius;
    const float ballMaxX = boxNode->position.x + (boxDimensions.x/2) - ballRadius;
    const float ballMinZ = boxNode->position.z - (boxDimensions.z/2) + ballRadius;
    const float ballMaxZ = boxNode->position.z + (boxDimensions.z/2) - ballRadius - cameraWallOffset;


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

        ballPosition.x = ballMinX + (1 - padPositionX) * (ballMaxX - ballMinX);
        ballPosition.y = ballBottomY;
        ballPosition.z = ballMinZ + (1 - padPositionZ) * ((ballMaxZ + cameraWallOffset) - ballMinZ);
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

            // Synchronize ball with music
            if (currentOrigin == BOTTOM && currentDestination == BOTTOM) {
                ballYCoord = ballBottomY;
            } else if (currentOrigin == TOP && currentDestination == TOP) {
                ballYCoord = ballBottomY + BallVerticalTravelDistance;
            } else if (currentDestination == BOTTOM) {
                ballYCoord = ballBottomY + BallVerticalTravelDistance * (1 - fractionFrameComplete);
            } else if (currentDestination == TOP) {
                ballYCoord = ballBottomY + BallVerticalTravelDistance * fractionFrameComplete;
            }

            // Make ball move
            const float ballSpeed = 60.0f;
            ballPosition.x += timeDelta * ballSpeed * ballDirection.x;
            ballPosition.y = ballYCoord;
            ballPosition.z += timeDelta * ballSpeed * ballDirection.z;

            // Make ball bounce
            if (ballPosition.x < ballMinX) {
                ballPosition.x = ballMinX;
                ballDirection.x *= -1;
            } else if (ballPosition.x > ballMaxX) {
                ballPosition.x = ballMaxX;
                ballDirection.x *= -1;
            }
            if (ballPosition.z < ballMinZ) {
                ballPosition.z = ballMinZ;
                ballDirection.z *= -1;
            } else if (ballPosition.z > ballMaxZ) {
                ballPosition.z = ballMaxZ;
                ballDirection.z *= -1;
            }

            if(options.enableAutoplay) {
                padPositionX = 1-(ballPosition.x - ballMinX) / (ballMaxX - ballMinX);
                padPositionZ = 1-(ballPosition.z - ballMinZ) / ((ballMaxZ+cameraWallOffset) - ballMinZ);
            }

            // Check if the ball is hitting the pad when the ball is at the bottom.
            // If not, you just lost the game! (hehe)
            if (jumpedToNextFrame && currentOrigin == BOTTOM && currentDestination == TOP) {
                double padLeftX  = boxNode->position.x - (boxDimensions.x/2) + (1 - padPositionX) * (boxDimensions.x - padDimensions.x);
                double padRightX = padLeftX + padDimensions.x;
                double padFrontZ = boxNode->position.z - (boxDimensions.z/2) + (1 - padPositionZ) * (boxDimensions.z - padDimensions.z);
                double padBackZ  = padFrontZ + padDimensions.z;

                if (   ballPosition.x < padLeftX
                    || ballPosition.x > padRightX
                    || ballPosition.z < padFrontZ
                    || ballPosition.z > padBackZ
                ) {
                    hasLost = true;
                    if (options.enableMusic) {
                        sound->stop();
                        delete sound;
                    }
                }
            }
        }
    }

    boxNode->position = { 0, -10, -80 };

    ballNode->position = ballPosition;
    ballNode->scale = glm::vec3(ballRadius);
    ballNode->rotation = { 0, totalElapsedTime*2, 0 };

    padNode->position  = {
        boxNode->position.x - (boxDimensions.x/2) + (padDimensions.x/2) + (1 - padPositionX) * (boxDimensions.x - padDimensions.x),
        boxNode->position.y - (boxDimensions.y/2) + (padDimensions.y/2),
        boxNode->position.z - (boxDimensions.z/2) + (padDimensions.z/2) + (1 - padPositionZ) * (boxDimensions.z - padDimensions.z)
    };


    //  "Animation" of light nodes
    for (size_t i = 0; i < padNode->children.size(); i++)
    {
        float offset = glm::two_pi<float>() * ((float)i / (NUM_LIGHTS - 1));
        padNode->children[i]->position.y = 2 + glm::cos(offset + totalElapsedTime) * 2;
        padNode->children[i]->position.x = 2 + glm::sin(offset + totalElapsedTime) * 2;
    }
    //lights[NUM_LIGHTS - 1].intensities = { 0.5f + glm::cos(totalElapsedTime * 4) * 0.5, 0,0.5f + glm::sin(totalElapsedTime * 4) * 0.5 };


    // Update geometry information for shadow casting
    glm::vec4 ballInfo = { 
        glm::vec3(ballNode->currentTransformationMatrix[3]), ballRadius
    };
    glm::mat2x3 boxInfo{
        glm::vec3(boxNode->currentTransformationMatrix[3]), boxDimensions * 0.5f
    };

    auto simpleGeometryShader = shaderSystem.get("simpleGeometry");
    simpleGeometryShader->bind();
    glUniform4fv(simpleGeometryShader->getUniformLocation("ball_info"), 1, glm::value_ptr(ballInfo));
    glUniformMatrix2x3fv(simpleGeometryShader->getUniformLocation("box_info"), 1, GL_FALSE, glm::value_ptr(boxInfo));

    auto texturedShader = shaderSystem.get("walls");
    texturedShader->bind();
    glUniform4fv(texturedShader->getUniformLocation("ball_info"), 1, glm::value_ptr(ballInfo));
    glUniformMatrix2x3fv(texturedShader->getUniformLocation("box_info"), 1, GL_FALSE, glm::value_ptr(boxInfo));


    updateNodeTransformations(rootNode);
}

void updateNodeTransformations(SceneNode* node) {
    updateNodeTransformations(node, glm::identity<glm::mat4>());
}
void updateNodeTransformations(SceneNode* node, glm::mat4 transformationThusFar) {
    glm::mat4 localTransformation;
    switch(node->nodeType) {
        case SceneNodeType::POINT_LIGHT: 
        {
            localTransformation = glm::translate(node->position);
            if (node->lightID != -1) {
                PointLight lightData = lights[node->lightID];

                auto packed = OK::pack_data(
                    glm::vec4{ glm::vec3{node->currentTransformationMatrix[3]}, 0 },
                    glm::vec4{ lightData.intensities, 1 },
                    lightData.constant,
                    lightData.linear,
                    lightData.quadratic,
                    0.0f
                );

                lightBuffer.update(sizeof(packed) * node->lightID, sizeof(packed), &packed);
            }
        }
        break;
        case SceneNodeType::SPOT_LIGHT: 
        {
            localTransformation =
                glm::translate(node->position)
                * glm::translate(node->referencePoint)
                * glm::rotate(node->rotation.y, glm::vec3(0, 1, 0))
                * glm::rotate(node->rotation.x, glm::vec3(1, 0, 0))
                * glm::rotate(node->rotation.z, glm::vec3(0, 0, 1))
                * glm::translate(-node->referencePoint);
        }
        break;
        default:
        {
            localTransformation =
                glm::translate(node->position)
                * glm::translate(node->referencePoint)
                * glm::rotate(node->rotation.y, glm::vec3(0, 1, 0))
                * glm::rotate(node->rotation.x, glm::vec3(1, 0, 0))
                * glm::rotate(node->rotation.z, glm::vec3(0, 0, 1))
                * glm::scale(node->scale)
                * glm::translate(-node->referencePoint);
        }
        break;
    }

    node->currentTransformationMatrix = transformationThusFar * localTransformation;

    for(SceneNode* child : node->children) {
        updateNodeTransformations(child, node->currentTransformationMatrix);
    }
}

void renderNode(SceneNode* node) {

    glm::mat4 projection = glm::perspective(glm::radians(80.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 350.f);

    glm::vec3 cameraPosition = glm::vec3(0, 2, -20);

    // Some math to make the camera move in a nice way
    float lookRotation = -0.6 / (1 + exp(-5 * (padPositionX - 0.5))) + 0.3;
    glm::mat4 cameraTransform =
        glm::rotate(0.3f + 0.2f * float(-padPositionZ * padPositionZ), glm::vec3(1, 0, 0)) *
        glm::rotate(lookRotation, glm::vec3(0, 1, 0)) *
        glm::translate(-cameraPosition);

    glm::mat4 VP = projection * cameraTransform;
    glm::mat4 MVP = VP * node->currentTransformationMatrix;
    glm::mat3 NRM = glm::transpose(glm::inverse(glm::mat3(node->currentTransformationMatrix)));
    switch(node->nodeType) {
        case SceneNodeType::GEOMETRY:
            if(node->vertexArrayObjectID != -1) {
                auto simpleGeometryShader = shaderSystem.get("simpleGeometry");
                simpleGeometryShader->bind();
                glUniformMatrix4fv(simpleGeometryShader->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
                glUniformMatrix4fv(simpleGeometryShader->getUniformLocation("TRS"), 1, GL_FALSE, glm::value_ptr(node->currentTransformationMatrix));
                glUniformMatrix3fv(simpleGeometryShader->getUniformLocation("NRM"), 1, GL_FALSE, glm::value_ptr(NRM));
                glUniform3fv(simpleGeometryShader->getUniformLocation("eye"), 1, glm::value_ptr(-cameraPosition));

                glBindVertexArray(node->vertexArrayObjectID);
                glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);
            }
            break;
        case SceneNodeType::UI:
            if (node->vertexArrayObjectID != -1) {
                auto uiShader = shaderSystem.get("UI");
                uiShader->bind();
                glm::mat4 ortho = glm::ortho<float>(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
                // Strictly speaking not the MVP, but this will allow us to reuse simple.vert
                glUniformMatrix4fv(uiShader->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(ortho * node->currentTransformationMatrix));
                glUniformMatrix4fv(uiShader->getUniformLocation("TRS"), 1, GL_FALSE, glm::value_ptr(node->currentTransformationMatrix));
                glUniformMatrix3fv(uiShader->getUniformLocation("NRM"), 1, GL_FALSE, glm::value_ptr(NRM));

                glBindVertexArray(node->vertexArrayObjectID);
                glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr); 
            }
            break;
        case SceneNodeType::NORMAL_MAPPED:
            if (node->vertexArrayObjectID != -1) {
                auto texturedShader = shaderSystem.get("walls");
                texturedShader->bind();
                glUniformMatrix4fv(texturedShader->getUniformLocation("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
                glUniformMatrix4fv(texturedShader->getUniformLocation("TRS"), 1, GL_FALSE, glm::value_ptr(node->currentTransformationMatrix));
                glUniformMatrix3fv(texturedShader->getUniformLocation("NRM"), 1, GL_FALSE, glm::value_ptr(NRM));
                glUniform3fv(texturedShader->getUniformLocation("eye"), 1, glm::value_ptr(-cameraPosition));

                glBindVertexArray(node->vertexArrayObjectID);
                glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);
            }
            break;
            break;
        case SceneNodeType::POINT_LIGHT: 
            break;
        case SceneNodeType::SPOT_LIGHT: break;
    }

    for(SceneNode* child : node->children) {
        renderNode(child);
    }
}

void renderFrame(GLFWwindow* window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    renderNode(rootNode);

    // Utility for grabbing a screenshot for the various tasks
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        OK::RawTexture imgGrab = OK::RawTexture();
        imgGrab.width = windowWidth;
        imgGrab.height = windowHeight;
        imgGrab.pixels.resize(4 * windowWidth * windowHeight);
        glReadPixels(0, 0, windowWidth, windowWidth, GL_RGBA, GL_UNSIGNED_BYTE, &imgGrab.pixels[0]);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cout << "Encountered error with glReadPixels: " << err << std::endl;
        savePNGFile("screenshot.png", imgGrab);
    }
}