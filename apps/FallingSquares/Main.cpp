#ifdef _DEBUG
#define WINDOW_MODE WindowMode::WINDOWED
#else
#define WINDOW_MODE WindowMode::FULLSCREEN
#endif // DEBUG

#define SCREEN_UNITS 100.0f
#define NUM_SQUARES 1000

#include <GL/glew.h>

#include "Square.hpp"

#include <TritiumEngine/Core/ResourceManager.hpp>
#include <TritiumEngine/Input/Keyboard.hpp>
#include <TritiumEngine/Rendering/RenderSystem.hpp>
#include <TritiumEngine/Rendering/ShaderLoaderFactory.hpp>
#include <TritiumEngine/UI/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

using namespace TritiumEngine::Core;
using namespace TritiumEngine::Input;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::UI;

typedef std::chrono::high_resolution_clock Clock;

int main() {
  // Setup GFLW properties
  if (glfwInit() == GLFW_FALSE) {
    printf("Error in initialising GLFW!\n");
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a window to use
  Window window("main", WINDOW_MODE);
  if (!window.IsInitialised()) {
    printf("Error in creating window!\n");
    return EXIT_FAILURE;
  }

  // Setup GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    printf("Error in initialising GLEW!\n");
    return EXIT_FAILURE;
  }

  // Setup resource paths
  ResourceManager<Shader>::Register(std::unique_ptr<ShaderLoaderFactory>(new ShaderLoaderFactory()),
                                    "Resources/Shaders/");

  // Setup camera
  float screenWidth = (float)window.GetWidth();
  float screenHeight = (float)window.GetHeight();
  float aspect = screenWidth / screenHeight;

  Camera camera(ProjectionType::ORTHOGRAPHIC, SCREEN_UNITS * aspect, SCREEN_UNITS, 0.1f, 100.0f);

  // Setup Renderer
  RenderSystem renderSystem;

  // Create squares
  Transform transform;
  transform.SetPosition(glm::vec3(-40.0f, 40.0f, 0.0f));
  transform.SetScale(glm::vec3(5.0f, 5.0f, 1.0f));

  std::vector<float> vertices = {
      0.5f,  0.5f,  // top right
      0.5f,  -0.5f, // bottom right
      -0.5f, -0.5f, // bottom left
      -0.5f, 0.5f   // top left
  };

  std::vector<unsigned int> indices = {
      0, 1, 2, // first triangle
      0, 2, 3  // second triangle
  };

  RenderData renderData{2, vertices, indices};

  const auto &shader = ResourceManager<Shader>::Create("default");
  const auto &material = std::make_shared<Material>(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f), shader);
  const auto &renderable = std::make_shared<Renderable>(GL_TRIANGLES, renderData);

  World world;
  for (int i = 0; i < 10; ++i) {
    world.AddEntity(new Square(transform, renderable, material));
    glm::vec3 pos = transform.GetPosition();
    transform.SetPosition({pos.x + 10, pos.y, pos.z});
  }

  // Setup timer
  auto prevTime = Clock::now();
  auto currentTime = Clock::now();
  float deltaTime;

  // Run application game loop
  int exitCode = EXIT_SUCCESS;
  try {
    while (!window.GetShouldClose()) {
      // Update clock
      prevTime = currentTime;
      currentTime = Clock::now();
      deltaTime = (currentTime - prevTime).count() / 1000000000.0f;

      window.Update();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

      // Keyboard controls
      if (Keyboard::GetKeyToggled(GLFW_KEY_ESCAPE))
        window.Close();

      // Update world and render
      world.Update(deltaTime);
      renderSystem.Draw(camera, world);
    }
  } catch (std::exception &e) {
    printf("%s", e.what());
    exitCode = EXIT_FAILURE;
  }

  world.ClearEntities();

  return exitCode;
}