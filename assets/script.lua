touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
accelState = Input.Get():GetAccel();
audioSystem = AudioSystem.Get();
renderer = Renderer.Get();
contentManager = ContentManager.Get();
gameObjectManager = GameObjectManager.Get();

angle = 0.0;
skocznia=0;
skocznia_drzewa=0;
skybox=0;
sound=0;


function drawProgress(bgTexture, progressTexture, region, progress)
	renderer:DrawSprite(400.0, 240.0, 10.0, 800.0, 480.0, region, bgTexture, 0.0);
	renderer:DrawSprite(0.0+800.0*progress/2, 20.0, 0.0, 800.0*progress , 40.0, region, progressTexture, 0);
	renderer:Flush();
end

loadAssets = function()
	percent=0.0;
	loaderScreen = contentManager:GetTextureManager():GetTexture(":loaderScreen.png");
	progressBar = contentManager:GetTextureManager():GetTexture(":progressBar.png");
	textureRegion = TextureRegion(0.0, 0.0, 1.0, 1.0);

	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading...");


        sound = ContentManager.Get():GetSoundManager():GetSound('sound.wav');
	percent=0.2;

	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading sounds");

	shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("perPixelLighting");
	vertexShader = contentManager:GetShaderManager():GetShader(":shaders/pixelLighting.vert");
	fragmentShader = contentManager:GetShaderManager():GetShader(":shaders/pixelLighting.frag");

        shaderProgram:Get():AddShader(vertexShader);
        shaderProgram:Get():AddShader(fragmentShader);
        shaderProgram:Get():Link();


        shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("perPixelLightingAlpha");
        vertexShader = contentManager:GetShaderManager():GetShader(":shaders/pixelLightingAlpha.vert");
        fragmentShader = contentManager:GetShaderManager():GetShader(":shaders/pixelLightingAlpha.frag");
        shaderProgram:Get():AddShader(vertexShader);
        shaderProgram:Get():AddShader(fragmentShader);
        shaderProgram:Get():Link();


	percent=0.4;

	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading shaders");




        gameObject1Id = gameObjectManager:AddObject(RenderableGameObject(Hash("gameObject1"), Vector3(0,0,0), Vector3(0,3.14,0), Vector3(0.5,0.5,0.5), "skocznia_teksury.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(gameObject1Id):SetShaderProgramParameter("lightPosition", Vector3(-50, 50, -50));

        gameObject2Id = gameObjectManager:AddObject(RenderableGameObject(Hash("gameObject2"), Vector3(0,0,0), Vector3(0,3.14,0), Vector3(0.5,0.5,0.5), "drzewa.ms3d", "perPixelLightingAlpha", true));
        gameObjectManager:FindObject(gameObject2Id):SetShaderProgramParameter("lightPosition", Vector3(-50, 50, -50));

	percent=0.8;
	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading geometry");

        skyboxId = gameObjectManager:AddObject(RenderableGameObject(Hash("skybox"), Vector3(0,30,0), Vector3(0,0,0), Vector3(15, 15, 15), "skybox.ms3d", "textured_3d", false));
        renderer:DrawString(10, 10, "Loading skybox");
	percent=1;
	drawProgress(loaderScreen, progressBar, textureRegion, percent);
end




function round(val, decimal)
  if (decimal) then
    return math.floor( (val * 10^decimal) + 0.5) / (10^decimal)
  else
    return math.floor(val+0.5)
  end
end


update = function(dt)
    if keysState:IsKeyJustPressed(Input.KEY_CENTER) then
        audioSystem:PlaySound(sound, 1.0);
    end

    if keysState:IsKeyJustPressed(Input.KEY_M) then
        audioSystem:SetMusicVolume(1.0);
    end
    if keysState:IsKeyJustPressed(Input.KEY_N) then
        audioSystem:SetMusicVolume(0.3);
    end

    if keysState:IsKeyPressed(Input.KEY_CENTER) then
        angle = 0;
    end

	camera = renderer:GetCamera();

    if keysState:IsKeyPressed(Input.KEY_UP) then
            camera:RotateUp(1*dt);
    elseif keysState:IsKeyPressed(Input.KEY_DOWN) then
            camera:RotateUp(-1*dt);
    elseif keysState:IsKeyPressed(Input.KEY_RIGHT) then
            camera:RotateLeft(1*dt);
    elseif keysState:IsKeyPressed(Input.KEY_LEFT) then
            camera:RotateLeft(-1*dt);

    elseif keysState:IsKeyPressed(Input.KEY_Z) then
            camera:MoveUp(2*dt);
    elseif keysState:IsKeyPressed(Input.KEY_X) then
            camera:MoveUp(-2*dt);

    elseif keysState:IsKeyPressed(Input.KEY_W) then
            camera:MoveForward(4*dt);
    elseif keysState:IsKeyPressed(Input.KEY_S) then
            camera:MoveForward(-4*dt);
    elseif keysState:IsKeyPressed(Input.KEY_A) then
            camera:MoveLeft(-4*dt);
    elseif keysState:IsKeyPressed(Input.KEY_D) then
            camera:MoveLeft(4*dt);
    end

    newSkyboxPosition = camera:GetPosition();
    newSkyboxPosition = newSkyboxPosition+Vector3(0,30,0);
    gameObjectManager:FindObject(skybox):SetPosition(newSkyboxPosition);


    if keysState:IsKeyJustPressed(Input.KEY_P) then
        audioSystem:PlayMusic("/sdcard/music.mp3", 1.0);
    end

        --gravity = accelState:GetAcceleration();
        --renderer:DrawString(0, 400, "Acceleration " .. round(gravity.x, 2) .. " " .. round(gravity.y, 2) .. " " .. round(gravity.z, 2));
        --gravity = accelState:GetRawAcceleration();
        --renderer:DrawString(0, 370, "Acceleration " .. round(gravity.x, 2) .. " " .. round(gravity.y, 2) .. " " .. round(gravity.z, 2));


    --Log("Acceleration " .. round(gravity.x, 2) .. " " .. round(gravity.y, 2) .. " " .. round(gravity.z, 2));

   -- translation = Matrix4x4();
   -- translation:SetTranslation(Vector3(-2,-1,-5));

--    rotation = Matrix4x4();
  --  rotation:SetScale(Vector3(2.0, 2.0, 2.0));

    --world = translation*rotation;

--    shaderParams = ShaderParametersList();
    --shaderParams:Add("lightPosition", Vector3(2*gravity.y, 2*gravity.x, 10));
  --  renderer:DrawGeometry(dwarfModel, world, shaderProgram, shaderParams);


    --translation:SetTranslation(Vector3(2, -1, -5));

--    world = translation*rotation;

  --  renderer:DrawGeometry(dwarfModel, world, shaderProgram, shaderParams);
         -- gameObjectManager:FindObject(gameObject1Id):SetShaderProgramParameter("lightPosition", Vector3(2*gravity.y, 2*gravity.x, 10));
       --   gameObjectManager:FindObject(gameObject2Id):SetShaderProgramParameter("lightPosition", Vector3(2*gravity.y, 2*gravity.x, 10));


    angle= angle + dt*30;
end
