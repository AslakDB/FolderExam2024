#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Camera.h"
#include "Model.h"
#include "Sphere.h"
#include "Collision.h"
#include "PointCloud.h"
#include <chrono>
#include <map>
#include <windows.h>

#include "grid.h"
#include "TrackingPath.h"

PointCloud pointCloud;
Sphere sphere;
Collision coll;
TrackingPath trackingPath;
// ABspline bsplineFunction;

Camera camera;
bool firstMouse = true;

float lastX = 960, lastY = 540;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void ProsessInput(GLFWwindow *window, float deltaTime, model& sphere, model& sphere2);
float calculateNormal(glm::vec3& vector_1, glm::vec3& vector_2);
bool calculateBarycentric(Vertex& P, Vertex& R, Vertex& Q, glm::vec3& PlayerPos, glm::vec3 normal);

struct Render
{
    bool inside;
    bool isMovingForward;
    
    Render() = default;
    void render(GLFWwindow* window, unsigned int shaderProgram, float deltaTime, float lastFrame) {

        float cellSize = 1.0f;
        
        glm::vec3 normal = glm::vec3(0.f);
        const float gravitiy = 9.81f ;
        float friction = pointCloud.friction;
        float mass = 4.f;
        
        
        model floorModel, ZWallP, ZWallN, XWallP, XWallN, PCloud, Track_Points;
        std::vector<model*> models = { &floorModel, &ZWallP, &ZWallN, &XWallP, &XWallN, &PCloud, &Track_Points };


      
        
        std::vector<model> sphereModels(20);
        std::vector<model*> sphere_models_ptr;

        
        std::vector<model> TrackPath(sphereModels.size());
        
        
        for (int i = 0; i < sphereModels.size(); ++i) {
            sphere_models_ptr.emplace_back(&sphereModels[i]);
            sphere.CreateSphere(sphereModels[i]);

        }


     
       std::vector<glm::vec3>  BSplinePoint;
        trackingPath.DrawPoints(Track_Points, BSplinePoint);
        
        
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 projection;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        pointCloud.CreatePlane( PCloud, "Trondheim_punkt_sky_comp6.txt");
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[seconds]" << std::endl;
        

         grid grid(pointCloud.minPoint, pointCloud.maxPoint, cellSize);
                for (const auto& triangle : PCloud.indices) {
                    grid.addTriangle(triangle, PCloud.vertices);
                }
        

        for (int i = 0; i < sphere_models_ptr.size(); i+=1)
        {
            
            sphere_models_ptr[i]->PlayerPos = glm::vec3(-5 - i, 1, 5 + i*2);
            sphere_models_ptr[i]->Velocity = glm::vec3(0.f,0.f,-8.f);
        }
        
        for (auto value : sphere_models_ptr)
        {
            value->Velocity = glm::vec3(-1.f,0.f,0.f);
        }
        std::map<int, std::vector<glm::vec3>> sphereControlPoints;


        
        int framecount = 0;
        int trackpoints = 0;
        while (!glfwWindowShouldClose(window))
        {
            framecount++;
            coll.SphereSphereCollision(sphere_models_ptr);
            

            if (framecount % 20 == 0)
            {
                trackpoints++;
                for (int i = 0; i < sphere_models_ptr.size(); ++i)
                {
                    sphere_models_ptr[i]->points.push_back(sphere_models_ptr[i]->PlayerPos);
                    trackingPath.DrawPoints(Track_Points, sphere_models_ptr[0]->points);

                    if (trackpoints > 3)
                    {
                        trackingPath.CreateBSplinePath(TrackPath[i], sphere_models_ptr[i]->points);
                        if (sphere_models_ptr[i]->points.size() > 34)
                        {
                            sphere_models_ptr[i]->points.erase(sphere_models_ptr[i]->points.begin());
                        }
                    }
                }
            }
            
            
            for (auto value : sphere_models_ptr)
            {
                sphere.Move(*value, deltaTime, gravitiy, friction, mass);
            }
            

            for (model* sphereModel : sphere_models_ptr) {
                glm::vec3 spherePosition = sphereModel->PlayerPos;
                std::vector<Triangle> relevantTriangles = grid.getTrianglesInCell(spherePosition);

                for (const auto& triangle : relevantTriangles) {
                    if (calculateBarycentric(PCloud.vertices[triangle.A], PCloud.vertices[triangle.B], PCloud.vertices[triangle.C], spherePosition, normal)) {
                        if (sphereModel->PlayerPos.y <= spherePosition.y && sphereModel->Velocity.y < 0) {
                            glm::vec3 normal = glm::normalize(glm::cross(PCloud.vertices[triangle.B].XYZ - PCloud.vertices[triangle.A].XYZ, PCloud.vertices[triangle.C].XYZ - PCloud.vertices[triangle.A].XYZ));
                            glm::vec3 velocityPerpendicular = glm::dot(sphereModel->Velocity , normal) * normal;
                            glm::vec3 velocityParallel = sphereModel->Velocity - velocityPerpendicular;

                            // Reduce the perpendicular component to simulate sliding
                            sphereModel->Velocity = velocityParallel - 0.2f * velocityPerpendicular ;
                            friction = PCloud.vertices[triangle.A].Friction;
                            // Move the sphere with the vertex friction
                            sphere.Move(*sphereModel, deltaTime, gravitiy, friction, mass);
                            if (sphereModel->PlayerPos.y < spherePosition.y)
                            {
                                sphereModel->PlayerPos.y = spherePosition.y;
                            }
                        }
                    }
                }
            }
                        
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                ProsessInput(window, deltaTime,  sphereModels[0], sphereModels[1]);
            

                projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

                camera.tick(shaderProgram);

       
                glClearColor(0.5f, 0.99f, 0.5f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                int viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
                glUniform3fv(viewLoc, 1, glm::value_ptr(camera.cameraPos));

                int LightLoc = glGetUniformLocation(shaderProgram, "lightPos");
                glUniform3fv(LightLoc, 1, glm::value_ptr(glm::vec3(0,10,0)));

                glLineWidth(3);
            glPointSize( 2.0f );

            //     glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

               

            for (model &element : TrackPath)
            {
                element.CalculateMatrix();
                element.CalculateBoundingBox();
                element.DrawMesh(shaderProgram);
            }
                for (model* element : models)
                {
                    element->CalculateMatrix();
                    element->CalculateBoundingBox();
                    //glPointSize(2.f);
                    element->DrawMesh(shaderProgram);
                }
            

            for (model& element : sphereModels)
                {
                    element.CalculateMatrix();
                    element.CalculateBoundingBox();
                    element.DrawMesh(shaderProgram);

                }
                
            
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


    void ProsessInput(GLFWwindow *window, float deltaTime, model& sphere, model& sphere2) {

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
            sphere.Velocity = glm::vec3(8.f,1.f,0.f);
        }

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            sphere.PlayerPos = camera.cameraPos;
            sphere.Velocity = camera.cameraFront * 8.f ;
            
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            sphere.PlayerPos = camera.cameraPos;
            
            sphere.Velocity = glm::vec3(0.f);
        }
        
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            sphere2.PlayerPos = camera.cameraPos;
            
            sphere2.Velocity = glm::vec3(0.f);
        }
       
    }


    float calculateNormal(glm::vec3& vector_1, glm::vec3& vector_2) {
        return vector_1[0]* vector_2[2]- vector_2[0]*vector_1[2];
    }

    bool calculateBarycentric(Vertex& P, Vertex& R, Vertex& Q, glm::vec3& PlayerPos, glm::vec3 normal) {
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
            float height=U* P.XYZ.y+ V * Q.XYZ.y + W * R.XYZ.y;
            PlayerPos.y = height + 0.5f;
            //calculate normal
            glm::vec3 Newnormal = glm::cross( Q.XYZ - P.XYZ, R.XYZ - P.XYZ);
            normal = Newnormal;
            
            return true;
        }
        return false;
    }

