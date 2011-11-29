contentManager = ContentManager.Get();

shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("textured_3d");
vertexShader = contentManager:GetShaderManager():GetShader(":shaders/3d.vert");
fragmentShader = contentManager:GetShaderManager():GetShader(":shaders/3d.frag");

shaderProgram:Get():AddShader(vertexShader);
shaderProgram:Get():AddShader(fragmentShader);
shaderProgram:Get():Link();


shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("spriteBatcher");
vertexShader = contentManager:GetShaderManager():GetShader(":shaders/batcher.vert");
fragmentShader = contentManager:GetShaderManager():GetShader(":shaders/batcher.frag");

shaderProgram:Get():AddShader(vertexShader);
shaderProgram:Get():AddShader(fragmentShader);
shaderProgram:Get():Link();

shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("nonTextured3d");
vertexShader = contentManager:GetShaderManager():GetShader(":shaders/nonTextured3d.vert");
fragmentShader = contentManager:GetShaderManager():GetShader(":shaders/nonTextured3d.frag");

shaderProgram:Get():AddShader(vertexShader);
shaderProgram:Get():AddShader(fragmentShader);
shaderProgram:Get():Link();
