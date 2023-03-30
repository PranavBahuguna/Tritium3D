#ifdef _DEBUG
#define WINDOW_MODE WindowMode::WINDOWED
#else
#define WINDOW_MODE WindowMode::FULLSCREEN
#endif // DEBUG

#define SCALE_FACTOR 100.0f

#include <GL/glew.h>

#include <TritiumEngine/Input/Keyboard.hpp>
#include <TritiumEngine/Rendering/Renderables/Mesh.hpp>
#include <TritiumEngine/Rendering/Renderer.hpp>
#include <TritiumEngine/UI/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace TritiumEngine::Input;
using namespace TritiumEngine::Rendering;
using namespace TritiumEngine::UI;

int main() {
  // Setup GFLW properties
  if (glfwInit() == GLFW_FALSE) {
    printf("Error in initialising GLFW!\n");
    return 1;
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

  // Load shader
  std::string vertexShader = "Resources/Shaders/default.vert";
  std::string fragmentShader = "Resources/Shaders/default.frag";
  Shader shader(vertexShader, fragmentShader);

  // Setup shader model projection matrix;
  shader.Use();
  float screenWidth = (float)window.GetWidth();
  float screenHeight = (float)window.GetHeight();

  glm::mat4 projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(screenWidth * 0.5f, screenHeight * 0.5f, 0.0f));
  model = glm::scale(model, glm::vec3(SCALE_FACTOR, SCALE_FACTOR, 1.0f));
  glm::mat4 modelProjection = projection * model;

  shader.SetMatrix4("modelProjection", modelProjection);

  // Create renderer
  Renderer renderer;

  std::vector<RenderList> polygons;

  std::vector<glm::vec2> vertices = {
      {0.5f, 0.5f},   // top right
      {0.5f, -0.5f},  // bottom right
      {-0.5f, -0.5f}, // bottom left
      {-0.5f, 0.5f},  // top left
  };

  std::vector<GLuint> indices = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  Mesh mesh(vertices, indices);
  RenderList renderList = {std::make_shared<Mesh>(mesh)};

  // Run application game loop
  try {
    while (!window.GetShouldClose()) {
      window.Update();
      renderer.Clear();

      // Keyboard controls
      if (Keyboard::GetKeyToggled(GLFW_KEY_ESCAPE))
        window.Close();

      // Render selected items
      renderer.Render(renderList, shader);
    }
  } catch (std::exception &e) {
    printf("%s", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}