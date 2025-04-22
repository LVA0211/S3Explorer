    #define GLM_ENABLE_EXPERIMENTAL
    #define _USE_MATH_DEFINES

    #include "imGUI/imgui.h"
    #include "imGUI/imgui_impl_glfw.h"
    #include "imGUI/imgui_impl_opengl3.h"

    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
    #include <iostream>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtx/string_cast.hpp>
    #include <glm/gtx/norm.hpp>
    #include <math.h>

    #include "VAO.h"
    #include "VBO.h"
    #include "EBO.h"
    #include "shaderClass.h"
    #include "Object.h"
    #include "Animations.h"



    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    bool looking = true;
    int current_slide;
    int total_slides;

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        ImGuiIO& io = ImGui::GetIO();

        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_ESCAPE) {
                looking = !looking;
                if (looking) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
                    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
                }
                else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

                    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
                    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
                }
            }

            if (key == GLFW_KEY_LEFT) {
                if (current_slide <= 0) {
                    current_slide = total_slides - 1;
                }
                else {
                    current_slide--;
                }
            }

            if (key == GLFW_KEY_RIGHT) {
                if (current_slide >= total_slides-1) {
                    current_slide = 0;
                }
                else {
                    current_slide++;
                }
            }
        }
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

    glm::mat4 get_rotcamera_matrix_from_angles(float yaw, float pitch) {
        float ca = cos(yaw);
        float sa = sin(yaw);
        float cb = cos(pitch);
        float sb = sin(pitch);

        glm::mat4 result = glm::mat4(
            ca, 0.f,-sa, 0.f,
            -sa*sb, cb, -ca*sb, 0.f,
            sa*cb, sb, ca*cb, 0.f,
            0.f,0.f,0.f, 1.f
        );

        return result;
    }

    glm::mat4 rotate4D(double angle, int axis1, int axis2) {
        glm::mat4 rotationMatrix(1.0f); // Identity matrix

        float cosA = cos(angle);
        float sinA = sin(angle);

        // Set 2D rotation in the selected plane
        rotationMatrix[axis1][axis1] = cosA;
        rotationMatrix[axis1][axis2] = -sinA;
        rotationMatrix[axis2][axis1] = sinA;
        rotationMatrix[axis2][axis2] = cosA;

        return rotationMatrix;
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


        const float inv_aspect_ratio = 9.f/16.f;
        const unsigned int window_width = 1920;
        const unsigned int window_height = round(window_width * inv_aspect_ratio);

        // Create window
        GLFWwindow* window = glfwCreateWindow(window_width, window_height, "S3", NULL, NULL);

        //Check if windows was created
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }


        //Do other shit
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
    
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //Initialize imGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = "imguisave";
        ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

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

        //---OBJECT CREATION--//
        std::vector<std::shared_ptr<Object>> allObjects = {
            std::make_shared<Object>("Utah Teapet"),
            std::make_shared<Object>("Grid"),
            std::make_shared<Object>("Globus"),
            std::make_shared<Object>("Gauss"),
            std::make_shared<Object>("Huis"),
        };
        //CUBE

        enum ObjectID {
            TEA_PET,   // 0
            TETRA_TILING,   // 1
            GLOBE,      // 2
            GAUSS, //3
            HOUSE, //4
        };

        allObjects[TEA_PET]->loadMesh("Meshes\\tiny_teapet.gltf");
        //allObjects[TEA_PET]->transform = glm::mat4(0.1f, 0.f, 0.f, 0.f, 0.f, 0.1f, 0.f, 0.f, 0.f, 0.f, 0.1f, 0.f, 0.f, 0.f, 0.f, 1.f);
        allObjects[TEA_PET]->setTransformFromSpherical(0.f,0.f,0.f);
        allObjects[TEA_PET]->scale = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

        allObjects[GLOBE]->loadMesh("Meshes\\globe.gltf");
        allObjects[GLOBE]->setScaleForUnitSphericalSphere(0.4f);
        allObjects[GLOBE]->setTransformFromSpherical(0.f, M_PI / 2.f, M_PI / 2.f);
        //allObjects[GLOBE]->animationFunc = rotateInPlane(1.0f, glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec4(1.f,0.f,0.f,0.f));

        allObjects[TETRA_TILING]->fromArray(
            std::vector<GLfloat>{
                1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
            },
            GL_LINES,
            std::vector<GLuint>{0, 1, 0, 2, 0, 3, 0, 5, 0, 6, 0, 7, 1, 2, 1, 3, 1, 4, 1, 6, 1, 7, 2, 3, 2, 4, 2, 5, 2, 7, 3, 4, 3, 5, 3, 6, 4, 5, 4, 6, 4, 7, 5, 6, 5, 7, 6, 7, },
            glm::vec3(0.f, 1.f, 0.f)
        );

        allObjects[GAUSS]->loadMesh("Meshes\\gauss.gltf");
        allObjects[GAUSS]->setTransformFromSpherical(0.f, 0.f, M_PI/2.f);
        allObjects[GAUSS]->scale = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);

        allObjects[HOUSE]->loadMesh("Meshes\\house.gltf");
        allObjects[HOUSE]->scale = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);
        allObjects[HOUSE]->setTransformFromSpherical(0.f,0.f,0.f);



        std::vector<std::vector<ObjectID>> slides = {
            {TEA_PET,GAUSS},
            {HOUSE},
            {TEA_PET}
        };

        total_slides = slides.size();
        current_slide = 0;

        //---MORE SETUP---//
        float blind_spot = 0.001f;
        float FOVX = M_PI/2.f;

        float w = tan(FOVX / 2.f);
        float h = w * inv_aspect_ratio;


        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 camera = glm::mat4(1.0f);
        glm::mat4 camera_rotation = glm::mat4(1.0f);

        glm::mat4 view;
        glm::mat4 proj_banana = glm::mat4(
            1.f/w, 0.f, 0.f, 0.f,
            0.f, 1.f/h, 0.f, 0.f,
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
        glm::mat4 to_antipode = glm::mat4(-1.f);

        //----------------------------
        //COmbining matrices for efficiency
        //----------------------------
        glm::mat4 banana_front = to_front_NDC * proj_banana;
        glm::mat4 banana_back = to_back_NDC * proj_banana;


        const float move_speed = 0.9f;

       
        float current_time = 0.0f;
        float delta = 0.0f;
        float last_time = glfwGetTime();
        unsigned int frame_count = 0;

        double xpos = 0.0, ypos = 0.0;
        double prevxpos = 0.0, prevypos = 0.0;
        glfwGetCursorPos(window, &prevxpos, &prevypos);
        double deltaX = 0.0, deltaY = 0.0;

        float yaw = 0.0f;
        float pitch = 0.0f;

        //-----------------------------------------//
        //------------imGUI variables--------------//
        //-----------------------------------------//
        float test_floats[] = { 0.f,0.f,0.f };

        float a = 0.f, b = 0.f, c = 0.f;
        float size = 1.f;
        float globe_radius = 0.5f;

        float light_falloff = 0.5f;
        float normal_falloff = 1.f;

        // Set uniform values for transformation matrices
        
        GLint modelLoc = shaderProgram.getUniformLocation("model");
        GLint modelScaleLoc = shaderProgram.getUniformLocation("scale");
        GLint viewLoc = shaderProgram.getUniformLocation("view");
        GLint projLoc = shaderProgram.getUniformLocation("projection");
        GLint samplerLoc = shaderProgram.getUniformLocation("diffuseTex");
        GLint usestextureboolLoc = shaderProgram.getUniformLocation("uses_texture");
        GLint diffusecolorLoc = shaderProgram.getUniformLocation("diffuse_color");
        GLint isbackhemisphereboolLoc = shaderProgram.getUniformLocation("is_back_hemisphere");

        GLint lightFalloffLoc = shaderProgram.getUniformLocation("light_falloff");
        GLint normalFalloffLoc = shaderProgram.getUniformLocation("normal_falloff");


        // Main render loop
        while (!glfwWindowShouldClose(window))
        {

            //Mouse things
            if (looking) {
                if (abs(deltaX) > 0.000001f || abs(deltaY) > 0.000001f) {
                    yaw += deltaX * delta;
                    pitch += deltaY * delta;

                    if (pitch > M_PI / 2.f) {
                        pitch = M_PI / 2.f;
                    }
                    else if (pitch < -M_PI / 2.f) {
                        pitch = -M_PI / 2.f;
                    }

                    camera_rotation = get_rotcamera_matrix_from_angles(yaw, pitch);
                }
            };

            //Move things

            glm::vec4 direction = get_direction_from_keys(window);

            if (glm::length2(direction) > 0.000001f) {

                direction = glm::normalize(direction);

                glm::mat4 rotmove = get_rotmove_matrix_from_direction(camera_rotation * direction, move_speed * delta);

                camera = camera * rotmove;

            }


            view = glm::inverse(camera* camera_rotation);

            allObjects[TEA_PET]->setTransformFromSpherical(a,b,c);
            allObjects[GLOBE]->setScaleForUnitSphericalSphere(globe_radius);

            shaderProgram.Activate();
            glClearColor(0.f,0.f,0.f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Let imGUI know that a new frame is in the making
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(banana_front));
            glUniform1i(isbackhemisphereboolLoc, 0);


            for (const auto& obj : allObjects) {
                if (obj->animationFunc) obj->animationFunc(*obj, current_time);
            };

            for (const auto& objID : slides[current_slide]) {
                allObjects[objID]->Draw(modelLoc, modelScaleLoc, samplerLoc, usestextureboolLoc, diffusecolorLoc);
            };

            view = to_antipode * view;
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(banana_back));
            glUniform1i(isbackhemisphereboolLoc, 1);

            for (const auto& objID : slides[current_slide]) {
                allObjects[objID]->Draw(modelLoc, modelScaleLoc, samplerLoc, usestextureboolLoc, diffusecolorLoc);
            };


            ImGui::Begin("Utah Teapot");
            ImGui::SliderFloat("a",&a,0.f,2*M_PI);
            ImGui::SliderFloat("b", &b, 0.f, 2 * M_PI);
            ImGui::SliderFloat("c", &c, 0.f, 2 * M_PI);
            ImGui::SliderFloat3("scale", &allObjects[TEA_PET]->scale[0], 0.1f, 3.f);
            ImGui::End();

            ImGui::Begin("Globe");
            ImGui::SliderFloat("radius", &globe_radius, 0.f, M_PI);
            ImGui::End();

            ImGui::Begin("Lighting");
            ImGui::Text("pitch = %f, yaw = %f", pitch, yaw);

            if (ImGui::SliderAngle("FOVx", &FOVX, 0.f, 179.f)) {
                w = tan(FOVX / 2.f);
                h = w * inv_aspect_ratio;
                proj_banana = glm::mat4(
                    1.f / w, 0.f, 0.f, 0.f,
                    0.f, 1.f / h, 0.f, 0.f,
                    0.f, 0.f, 0.f, -1.f,
                    0.f, 0.f, -blind_spot, 0.f
                );
                banana_front = to_front_NDC * proj_banana;
                banana_back = to_back_NDC * proj_banana;
            };
            //ImGui::SliderFloat("Blind Spot", &blind_spot, 0.001f, 0.1f)
            if (ImGui::SliderFloat("light falloff factor", &light_falloff, 0.f, 1.f)) glUniform1f(lightFalloffLoc,light_falloff);
            if (ImGui::SliderFloat("normal falloff factor", &normal_falloff, 0.f, 2.f)) glUniform1f(normalFalloffLoc, normal_falloff);
            ImGui::End();

            ImGui::Begin("Slide Controls");

            if (ImGui::Button("Prev") && current_slide > 0) {
                current_slide--;
            }

            ImGui::SameLine();

            if (ImGui::Button("Next") && current_slide < total_slides - 1) {
                current_slide++;
            }

            //Scrollable slide selector
            ImGui::BeginListBox("Slides");
            for (int i = 0; i < total_slides; ++i) {
                bool isSelected = (current_slide == i);
                std::string label = "Slide " + std::to_string(i);
                if (ImGui::Selectable(label.c_str(), &isSelected)) {
                    current_slide = i;
                };
            };
            ImGui::EndListBox();

            ImGui::End();


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
            
            //Register all inputs + Recalc time
            
            current_time = glfwGetTime();

            delta = current_time - last_time;
            last_time = current_time;

            glfwGetCursorPos(window, &xpos, &ypos);
            deltaX = xpos - prevxpos;
            deltaY = ypos - prevypos;

            prevxpos = xpos;
            prevypos = ypos;


            frame_count++;
        }

        allObjects.clear();
        shaderProgram.Delete();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
        return 0;
    }
