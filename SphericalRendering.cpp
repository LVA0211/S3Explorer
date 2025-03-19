    #define GLM_ENABLE_EXPERIMENTAL

    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
    #include <iostream>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtx/string_cast.hpp>
    #include <glm/gtx/norm.hpp>

    #include "VAO.h"
    #include "VBO.h"
    #include "EBO.h"
    #include "shaderClass.h"
    #include "Object.h"


    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    glm::vec4 get_direction_from_keys(GLFWwindow* window) {
        int W = glfwGetKey(window, GLFW_KEY_W);
        int A = glfwGetKey(window, GLFW_KEY_A);
        int S = glfwGetKey(window, GLFW_KEY_S);
        int D = glfwGetKey(window, GLFW_KEY_D);
        int SPACE = glfwGetKey(window, GLFW_KEY_SPACE);
        int SHIFT = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        glm::vec4 result = glm::vec4(0.0f);

        if (W == GLFW_PRESS) {
            result.z -= 1.0f;
        }
        if (S == GLFW_PRESS) {
            result.z += 1.0f;
        }
        if (A == GLFW_PRESS) {
            result.x += 1.0f;
        }
        if (D == GLFW_PRESS) {
            result.x -= 1.0f;
        }

        if (SPACE == GLFW_PRESS) {
            result.y += 1.0f;
        }
        if (SHIFT == GLFW_PRESS) {
            result.y -= 1.0f;
        }

        return result;
    }

    glm::mat4 get_rotmove_matrix_from_direction(glm::vec4 direction, float angle) {
        glm::mat4 firstMat = glm::mat4(0.0f);

        firstMat[0][3] = -direction.x;
        firstMat[1][3] = -direction.y;
        firstMat[2][3] = -direction.z;

        firstMat[3][0] = direction.x;
        firstMat[3][1] = direction.y;
        firstMat[3][2] = direction.z;

        glm::mat4 secondMat = glm::outerProduct(direction, direction);
    
        return glm::mat4(1.0f) + sin(angle) * firstMat + (cos(angle) - 1.0f) * secondMat;
    }

    int main()
    {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;
        }
        // OpenGL version and profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
        GLFWwindow* window = glfwCreateWindow(800, 600, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", NULL, NULL);

        //Check if windows was created
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }

        //Do other shit
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Load OpenGL function pointers with GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        // Enable depth
        glEnable(GL_DEPTH_TEST);

        // Compile vertex shader
        Shader shaderProgram("default.vert", "default.frag");

        //---End of Setup--//

        Object test_obj;
        test_obj.loadMesh("Meshes\\tiny_teapet.gltf");
        test_obj.transform = glm::mat4(0.1f, 0.f, 0.f, 0.f, 0.f, 0.1f, 0.f, 0.f, 0.f, 0.f, 0.1f, 0.f, 0.f, 0.f, 0.f, 1.f);

        // Triangle vertices
        GLfloat vertices_test_cube[] = {
            // Position of vertices (x, y, z)
            0.1f,  0.1f,  0.1f, 1.0f,   1.f, 1.f, 1.f,
            0.1f,  0.1f, -0.1f, 1.0f,    1.f, 1.f, 0.f,
            0.1f, -0.1f,  0.1f, 1.0f,    1.f, 0.f, 1.f,
            0.1f, -0.1f, -0.1f, 1.0f,    1.f, 0.f, 0.f,
            -0.1f, 0.1f,  0.1f, 1.0f,    0.f, 1.f, 1.f,
            -0.1f, 0.1f, -0.1f, 1.0f,    0.f, 1.f, 0.f,
            -0.1f,-0.1f,  0.1f, 1.0f,    0.f, 0.f, 1.f,
            -0.1f,-0.1f, -0.1f, 1.0f,    0.f, 0.f, 0.f,
        };

        GLfloat vertices[] = {
            // Position of vertices (x, y, z)
            0.07f, 0.0f, 0.f, 1.0f,   1.f, 0.f, 0.f,
            -0.07f, 0.1f, 0.f, 1.0f,   1.f, 0.f, 0.f,
            -0.07f, -0.1f, 0.f, 1.0f,   1.f, 0.f, 0.f,

            0.07f, 0.0f, 0.3f, 1.0f,   0.f, 0.f, 1.f,
            -0.07f, 0.1f, 0.3f, 1.0f,   0.f, 0.f, 1.f,
            -0.07f, -0.1f, 0.3f, 1.0f,   0.f, 0.f, 1.f,

            1.f,0.f,0.f,0.f, 1.f,0.f,0.f,
            0.f,1.f,0.f,0.f, 0.f,1.f,0.f,
            0.f,0.f,1.f,0.f, 0.f,0.f,1.f,
        };

        GLuint indices[] = {
            0,1,2,
            3,4,5,
            6,7,8
        };

        GLuint indices_test_cube[] = {
            0,1,3,
            3,2,0,

            4,5,7,
            7,6,4,

            0,1,5,
            5,4,0,

            2,3,7,
            7,6,2,

            0,2,6,
            6,4,0,

            1,3,7,
            7,5,1,
        };

        VAO VAO1;
        VAO1.Bind();

        VBO VBO1;
        VBO1.SetBufferData(vertices, sizeof(vertices));
        EBO EBO1;
        EBO1.SetBufferData(indices, sizeof(indices));

        VAO1.linkAttrib(VBO1, 0, 4, GL_FLOAT, 7 * sizeof(float), (void*)0);
        VAO1.linkAttrib(VBO1, 1, 3, GL_FLOAT, 7 * sizeof(float), (void*)(4*sizeof(float)));

        VAO1.Unbind();
        VBO1.Unbind();
        EBO1.Unbind();

        const float blind_spot = 0.001f;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 camera = glm::mat4(1.0f);
        glm::mat4 view;
        glm::mat4 proj_banana = glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 0.f, -1.f,
            0.f, 0.f, -blind_spot, 0.f
        );

        //FRONT HEMISPHERE

        //-1<z<1 to -1<z<0 --> z*1/2 - 1/2
        glm::mat4 to_front_NDC = glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 0.5f, 0.f,
            0.f, 0.f, -0.5f, 1.f
        );

        //no modification to camera mat

        //BACK HEMISPHERE
        //-1<z<1 to 0<z<1
        glm::mat4 to_back_NDC = glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 0.5f, 0.f,
            0.f, 0.f, 0.5f, 1.f
        );
        //rotate by pi in wz
        glm::mat4 to_antipode = glm::mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, -1.f, 0.f,
            0.f, 0.f, 0.f, -1.f
        );

        //----------------------------
        //COmbining matrices for efficiency
        //----------------------------
        glm::mat4 banana_front = to_front_NDC * proj_banana;
        glm::mat4 banana_back = to_back_NDC * proj_banana;


        const float step_size = 0.000001f;
        const float move_speed = 0.3f;

        float delta = 0.0f;
        float last_time = 0.0f;
        unsigned int frame_count = 0;

        // Set uniform values for transformation matrices
        GLint modelLoc = shaderProgram.getUniformLocation("model");
        GLint viewLoc = shaderProgram.getUniformLocation("view");
        GLint projLoc = shaderProgram.getUniformLocation("projection");
        GLint samplerLoc = shaderProgram.getUniformLocation("diffuseTex");
        GLint usestextureboolLoc = shaderProgram.getUniformLocation("uses_texture");
        GLint diffusecolorLoc = shaderProgram.getUniformLocation("diffusecolorLoc");
        GLint isbackhemisphereboolLoc = shaderProgram.getUniformLocation("is_back_hemisphere");

        // Main render loop
        while (!glfwWindowShouldClose(window))
        {

            //Calculate deltatime
            float current_time = glfwGetTime();
            delta = current_time - last_time;
            last_time = current_time;
       
            shaderProgram.Activate();

            glm::vec4 direction = get_direction_from_keys(window);

            if (glm::length2(direction) > 0.000001f) {

                direction = glm::normalize(direction);

                glm::mat4 rotmove = get_rotmove_matrix_from_direction(direction, move_speed * delta);

                camera = camera * rotmove;

            }
            view = glm::inverse(camera);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(banana_front));
            glUniform1i(isbackhemisphereboolLoc, 0);


            test_obj.Draw(modelLoc, samplerLoc, usestextureboolLoc, diffusecolorLoc);

            view = to_antipode * view;
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(banana_back));
            glUniform1i(isbackhemisphereboolLoc, 1);

            test_obj.Draw(modelLoc, samplerLoc, usestextureboolLoc, diffusecolorLoc);

            glfwSwapBuffers(window);
            glfwPollEvents();
        

            //std::cout << glm::to_string(camera[3]) << std::endl;
            frame_count++;
        }

        // Cleanup
        VAO1.Delete();
        VBO1.Delete();
        EBO1.Delete();
        shaderProgram.Delete();

        glfwTerminate();
        return 0;
    }
