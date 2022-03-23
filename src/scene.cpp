#include "scene.h"

Scene::Scene()
{
    width = 800;
    height = 600;
    init();
}

void Scene::init()
{
    glEnable(GL_CULL_FACE);
    //cube VAO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeVertices.size(), &cubeVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    //set up matrix
    model = glm::mat4(1.0f);
    view = camera.GetViewMatrix();
    projection = glm::perspective(camera.Zoom, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    
    renderShader.use();
    renderShader.setMat4("model", model);
    renderShader.setMat4("view", view);
    renderShader.setMat4("projection", projection);

    //3d texture
    setupTex3d();

    //compute tex3d
    computeTex3d();
}

void Scene::setupTex3d()
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex3D);
    glBindTexture(GL_TEXTURE_3D, tex3D);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, texWidth, texHeight, texDepth, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindImageTexture(0, tex3D, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void Scene::computeTex3d()
{
    computeShader.use();
    glDispatchCompute(texWidth, texHeight, texDepth);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Scene::draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderShader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
