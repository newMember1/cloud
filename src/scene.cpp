#include "scene.h"

Scene::Scene()
{
    width = 800;
    height = 600;
    init();
}

void Scene::pushQuad(float xmin, float xmax, float zmin, float zmax, float y)
{
    //only store vertex position
    //1 2 3
    cubeVertices.push_back(xmin);
    cubeVertices.push_back(y);
    cubeVertices.push_back(zmin);

    cubeVertices.push_back(xmin);
    cubeVertices.push_back(y);
    cubeVertices.push_back(zmax);

    cubeVertices.push_back(xmax);
    cubeVertices.push_back(y);
    cubeVertices.push_back(zmax);

    //3 4 1
    cubeVertices.push_back(xmax);
    cubeVertices.push_back(y);
    cubeVertices.push_back(zmax);

    cubeVertices.push_back(xmax);
    cubeVertices.push_back(y);
    cubeVertices.push_back(zmin);

    cubeVertices.push_back(xmin);
    cubeVertices.push_back(y);
    cubeVertices.push_back(zmin);
}

void Scene::initCubeDatas()
{
    cubeVertices.clear();
    float xmin = -0.5;
    float xmax = 0.5;
    float ymin = -0.5;
    float ymax = 0.5;
    float zmin = -0.5;
    float zmax = 0.5;

    //from ymin to ymax generate data
    int cnt = 60;
    float delta = (ymax - ymin) / cnt;
    for(int i = 0; i < cnt; ++i)
    {
        float tY = ymin + delta * i;
        pushQuad(xmin, xmax, zmin, zmax, tY);
    }
}

void Scene::init()
{
    //global settings
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //cube datas
    initCubeDatas();

    //cube VAO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeVertices.size(), &cubeVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

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
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size() / 3);
}
