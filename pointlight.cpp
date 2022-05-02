#include "pointlight.h"
#include "shader.h"
PointLight::PointLight(Shader& shader, int pointLightIndex) : Light(shader)
{
    mPointLightIndex = pointLightIndex;
    mMatrix.setToIdentity();
}
PointLight::PointLight(Shader& shader, Texture* texture,int pointLightIndex) : Light(shader)
{
    mPointLightIndex = pointLightIndex;
    mTexture = texture;
    mMatrix.setToIdentity();
}
void PointLight::init(){
    Light::init();
}

void PointLight::draw(){
    //Does point light thing
    //In my lightshader, set my matrix posistion to the pointLights index with my index
    //The light shader has to know every frame where my point light posistion is becasuse it may change from frame to fram
    //Use the shader to change the variables in it???? Everything turns black if .use() isnt called
    mShader.use();
    mShader.SetUniform3f(mPosition.column(3).x(), mPosition.column(3).y(), mPosition.column(3).z(),
                           "pointLights[" + std::to_string(mPointLightIndex) + "].position");
    //Give it ambient
    mShader.SetUniform3f(mLightColor.mX * mAmbientStrength, mLightColor.mY * mAmbientStrength, mLightColor.mZ *  mAmbientStrength,
                           "pointLights[" + std::to_string(mPointLightIndex) + "].ambient");
    //Diffuse
    mShader.SetUniform3f(mLightColor.mX, mLightColor.mY, mLightColor.mZ,
                           "pointLights[" + std::to_string(mPointLightIndex) + "].diffuse");
    //Maybe specular, should maybe be per object
    mShader.SetUniform3f( 1.f, 1.f, 1.f, "pointLights[" + std::to_string(mPointLightIndex) + "].specular");
    //Constant?
    mShader.SetUniform1f(1.0f, "pointLights[" + std::to_string(mPointLightIndex) + "].constant");
    //Linear
    mShader.SetUniform1f(0.09f, "pointLights[" + std::to_string(mPointLightIndex) + "].linear");
    //Quadtratic
    mShader.SetUniform1f(0.032f, "pointLights[" + std::to_string(mPointLightIndex) + "].quadtratic");

    Light::draw();
}
