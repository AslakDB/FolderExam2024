#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Camera.h"
#include "Model.h"
#include "Sphere.h"
#include "Collision.h"
#include "PointCloud.h"
#include <chrono>

PointCloud pointCloud;
Sphere sphere;
Collision coll;

// ABspline bsplineFunction;

Camera camera;
bool firstMouse = true;

float lastX = 960, lastY = 540;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void ProsessInput(GLFWwindow *window, float deltaTime, model& sphere);
float calculateNormal(glm::vec3& vektor1, glm::vec3& vektor2);
bool calculateBarycentric(Vertex& P, Vertex& R, Vertex& Q, glm::vec3& PlayerPos);

struct Render
{
    bool inside;
    bool isMovingForward;
    
    Render() = default;
    void render(GLFWwindow* window, unsigned int shaderProgram, float deltaTime, float lastFrame) {
         
        model SphereModel0, SphereModel1, SphereModel2, SphereModel3, SphereModel4;
        
        model floorModel, ZWallP, ZWallN, XWallP, XWallN, PCloud;
        std::vector<model*> models = { &floorModel, &ZWallP, &ZWallN, &XWallP, &XWallN, &PCloud};

       
        std::vector<model*> sphere_models;

        
        
        sphere_models.emplace_back(&SphereModel0);
        sphere_models.emplace_back(&SphereModel1);
        sphere_models.emplace_back(&SphereModel2);
        sphere_models.emplace_back(&SphereModel3);
        sphere_models.emplace_back(&SphereModel4);
     

        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 projection;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        pointCloud.CreatePlane( PCloud, "Trondheim_punkt_sky_comp6.txt");
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[seconds]" << std::endl;

        
        sphere.CreateSphere(SphereModel0);
        sphere.CreateSphere(SphereModel1);
        sphere.CreateSphere(SphereModel2);
        sphere.CreateSphere(SphereModel3);
        sphere.CreateSphere(SphereModel4);
        
        
        floorModel.PlayerPos = glm::vec3(0.f,0.f,0.f);
       
        ZWallN.PlayerPos= glm::vec3(0.f, 0, -4.5f);
        ZWallN.PlayerRotation = glm::vec3(90.f,0.f,0.f);
        ZWallN.PlayerScale = glm::vec3(1.f,1.f,0.1f);
        
        ZWallP.PlayerPos= glm::vec3(0.f, 0.f, 4.5f);
        ZWallP.PlayerRotation = glm::vec3(-90.f,0.f,0.f);
        ZWallP.PlayerScale = glm::vec3(1.f,1.f,0.1f);
        
        XWallN.PlayerPos= glm::vec3(-4.5f, 0.f, 0.f);
        XWallN.PlayerRotation = glm::vec3(0.f,0.f,-90.f);
        XWallN.PlayerScale = glm::vec3(0.1f,1.f,1.f);
        
        XWallP.PlayerPos= glm::vec3(4.5f, 0.f, 0.f);
        XWallP.PlayerRotation = glm::vec3(0.f,0.f,90.f);
        XWallP.PlayerScale = glm::vec3(0.1f,1.f,1.f);

        SphereModel0.PlayerPos = glm::vec3(-1.f,1.f,-1.f);
        SphereModel1.PlayerPos = glm::vec3(-6.f,1.f,5.2f);
        std::cout<< SphereModel1.PlayerPos.y <<std::endl;
        SphereModel2.PlayerPos = glm::vec3(0.f,0.1f,-1.f);
        SphereModel3.PlayerPos = glm::vec3(2.f,-0.1f,0.f);
        SphereModel4.PlayerPos = glm::vec3(1.f,0.1f,-1.f);


        
        while (!glfwWindowShouldClose(window))
        {

            coll.SphereSphereCollision(sphere_models);
            coll.SphereBoxCollision(sphere_models,models);

            
            sphere.Move(SphereModel0, deltaTime, SphereModel0.Velocity);
            sphere.Move(SphereModel1, deltaTime, glm::vec3(-1.f,0.f,0.f));
            sphere.Move(SphereModel2, deltaTime, SphereModel2.Velocity);
            sphere.Move(SphereModel3, deltaTime, SphereModel3.Velocity);
            sphere.Move(SphereModel4, deltaTime, SphereModel4.Velocity);


            /*for (auto Spheres : sphere_models)
            {}*/
                for (auto element : PCloud.indices)
                {
                    //check if a ball is inside the plane
                    calculateBarycentric( PCloud.vertices[element.A],PCloud.vertices[element.B],PCloud.vertices[element.C], SphereModel1.PlayerPos);
                }
            

           std::cout<<SphereModel1.PlayerPos.x<<", " << SphereModel1.PlayerPos.y<<", " << SphereModel1.PlayerPos.z <<std::endl;
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                ProsessInput(window, deltaTime, ZWallN);
            

                projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

                camera.tick(shaderProgram);

       
                glClearColor(0.5f, 0.99f, 0.5f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                int viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
                glUniform3fv(viewLoc, 1, glm::value_ptr(camera.cameraPos));

                int LightLoc = glGetUniformLocation(shaderProgram, "lightPos");
                glUniform3fv(LightLoc, 1, glm::value_ptr(glm::vec3(0,10,0)));

                glLineWidth(3);

                // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            
                for (model* element : sphere_models)
                {
                    element->CalculateMatrix();
                    element->CalculateBoundingBox();
                    element->DrawMesh(shaderProgram);

                }
                for (model* element : models)
                {
                    element->CalculateMatrix();
                    element->CalculateBoundingBox();
                    //glPointSize(2.f);
                    element->DrawMesh(shaderProgram);
                }

                // bsplineModel2.CalculateMatrix();
                // bsplineModel2.DrawMesh(shaderProgram);
            
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }
    };

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.yaw   += xoffset;
        camera.pitch += yoffset;

        if(camera.pitch > 89.0f)
            camera.pitch = 89.0f;
        if(camera.pitch < -89.0f)
            camera.pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        direction.y = sin(glm::radians(camera.pitch));
        direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        camera.cameraFront = glm::normalize(direction);
    }


    void ProsessInput(GLFWwindow *window, float deltaTime, model& sphere) {

        glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(camera.cameraFront.x, 0.0f, camera.cameraFront.z));

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float cameraSpeed = 2.5f * deltaTime ;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.cameraPos += cameraSpeed * cameraFrontXZ;
            /*Player.PlayerPos +=  cameraSpeed * cameraFrontXZ;*/}
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        { camera.cameraPos -= cameraSpeed * cameraFrontXZ;
            /*Player.PlayerPos -=  cameraSpeed * cameraFrontXZ;*/}
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
            /*Player.PlayerPos -=  glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;*/}
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
            /*Player.PlayerPos +=  glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;*/}
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.cameraPos += cameraSpeed * camera.cameraUp; // Move camera up
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.cameraPos -= cameraSpeed * camera.cameraUp;

        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            std::cout<<"move forward"<<std::endl;
            sphere.Velocity = glm::vec3(8.f,0.f,0.f);
        }
    
    }


    float calculateNormal(glm::vec3& vektor1, glm::vec3& vektor2) {
        return vektor1[0]* vektor2[2]- vektor2[0]*vektor1[2];
    }

    bool calculateBarycentric(Vertex& P, Vertex& R, Vertex& Q, glm::vec3& PlayerPos) {
        glm::vec3 x1 = Q.XYZ - P.XYZ;
        glm::vec3 x2 = R.XYZ - P.XYZ;
        float Areal = calculateNormal(x1,x2);
        
        glm::vec3 u1 = Q.XYZ- PlayerPos;
        glm::vec3 u2 = R.XYZ- PlayerPos;

        float U = calculateNormal(u1,u2) /Areal;

        glm::vec3 v1 = R.XYZ - PlayerPos;
        glm::vec3 v2 = P.XYZ - PlayerPos;

        float V = calculateNormal(v1,v2)/ Areal;

        float W = 1 - U - V;

        if (U >=0 && V >= 0 && W >=0)
        {
            std::cout << U << ", "<<V<<", "<<W << std::endl;
            float height=U* P.XYZ.y+ V * Q.XYZ.y + W * R.XYZ.y;
            std::cout << height << std::endl;
            PlayerPos.y = height + 0.5f;
            return true;
        }
        return false;
    }

