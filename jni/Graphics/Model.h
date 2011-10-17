#ifndef MODEL_H
#define MODEL_H

class IGeometry;
class VBO;
class Shader;
class ShaderProgram;

class Model
{
public:
    Model();

    void Load(const char* filename);

private:
    IGeometry* geometry;
    VBO* vbo;
    Shader* vertexShader;
    Shader* fragmentShader;
    ShaderProgram* shaderProgram;
};

#endif // MODEL_H
