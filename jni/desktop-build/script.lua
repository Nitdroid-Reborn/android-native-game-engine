touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
audioSystem = AudioSystem.Get();
renderer = Renderer.Get();
contentManager = ContentManager.Get();
gameObjectManager = GameObjectManager.Get();


local clock = os.clock
function sleep(n)  -- seconds
  local t0 = clock()
  while clock() - t0 <= n do end
end

function round(val, decimal)
  if (decimal) then
    return math.floor( (val * 10^decimal) + 0.5) / (10^decimal)
  else
    return math.floor(val+0.5)
  end
end

function drawProgress(bgTexture, progressTexture, region, progress)
	renderer:DrawSprite(400.0, 240.0, 10.0, 800.0, 480.0, region, bgTexture, 0.0);
	renderer:DrawSprite(0.0+800.0*progress/2, 20.0, 0.0, 800.0*progress , 40.0, region, progressTexture, 0);
	renderer:Flush();
end

loadAssets = function()
        shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("perPixelLighting");
        vertexShader = contentManager:GetShaderManager():GetShader("shaders/pixelLighting.vert");
        fragmentShader = contentManager:GetShaderManager():GetShader("shaders/pixelLighting.frag");

        shaderProgram:Get():AddShader(vertexShader);
        shaderProgram:Get():AddShader(fragmentShader);
        shaderProgram:Get():Link();


        shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("perPixelLightingAlpha");
        vertexShader = contentManager:GetShaderManager():GetShader("shaders/pixelLightingAlpha.vert");
        fragmentShader = contentManager:GetShaderManager():GetShader("shaders/pixelLightingAlpha.frag");
        shaderProgram:Get():AddShader(vertexShader);
        shaderProgram:Get():AddShader(fragmentShader);
        shaderProgram:Get():Link();

        skocznia = gameObjectManager:AddObject(RenderableGameObject(Hash("skocznia"), Vector3(0,0,0), Vector3(0,0,0), Vector3(0.5,0.5,0.5), "skocznia_teksury.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(skocznia):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));


        skybox = gameObjectManager:AddObject(RenderableGameObject(Hash("skybox"), Vector3(0,30,0), Vector3(0,0,0), Vector3(15, 15, 15), "skybox.ms3d", "textured_3d", false));

        camera = renderer:GetCamera();
        camera:SetPosition(Vector3(0, 5, -11));
end



update = function(dt)
    camera = renderer:GetCamera();


    if keysState:IsKeyPressed(Input.KEY_UP) then
            camera:RotateUp(1*dt);
    end
    if keysState:IsKeyPressed(Input.KEY_DOWN) then
            camera:RotateUp(-1*dt);
    end
    if keysState:IsKeyPressed(Input.KEY_RIGHT) then
            camera:RotateLeft(1*dt);
    end
    if keysState:IsKeyPressed(Input.KEY_LEFT) then
            camera:RotateLeft(-1*dt);
    end

    if keysState:IsKeyPressed(Input.KEY_Z) then
            camera:MoveUp(2*dt);
    end
    if keysState:IsKeyPressed(Input.KEY_X) then
            camera:MoveUp(-2*dt);
    end

    if keysState:IsKeyPressed(Input.KEY_W) then
            camera:MoveForward(4*dt);
    end
    if keysState:IsKeyPressed(Input.KEY_S) then
            camera:MoveForward(-4*dt);
    end
    if keysState:IsKeyPressed(Input.KEY_A) then
            camera:MoveLeft(-4*dt);
    end
    if keysState:IsKeyPressed(Input.KEY_D) then
            camera:MoveLeft(4*dt);
    end

    newSkyboxPosition = camera:GetPosition();
    newSkyboxPosition = newSkyboxPosition+Vector3(0,10,0);
    gameObjectManager:FindObject(skybox):SetPosition(newSkyboxPosition);

end
