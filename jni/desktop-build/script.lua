touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
audioSystem = AudioSystem.Get();
renderer = Renderer.Get();
contentManager = ContentManager.Get();
gameObjectManager = GameObjectManager.Get();

angle = 0.0;
skocznia=0;
skocznia_drzewa=0;
skocznia_fizyka=0;
nartaLewa=0;
nartaPrawa=0;
skybox=0;

dofile("ski.lua");

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
	percent=0.0;
	loaderScreen = contentManager:GetTextureManager():GetTexture("loaderScreen.png");
	progressBar = contentManager:GetTextureManager():GetTexture("progressBar.png");
	textureRegion = TextureRegion(0.0, 0.0, 1.0, 1.0);

	drawProgress(loaderScreen, progressBar, textureRegion, percent);

	sound = ContentManager.Get():GetSoundManager():GetSound('/sdcard/violin.wav');
	percent=0.2;

	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading sounds");


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

        percent=0.4;
        drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading shaders");


        skocznia = gameObjectManager:AddObject(RenderableGameObject(Hash("skocznia"), Vector3(0,0,0), Vector3(0,0,0), Vector3(0.5,0.5,0.5), "skocznia_teksury.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(skocznia):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));

        skocznia_drzewa = gameObjectManager:AddObject(RenderableGameObject(Hash("skocznia_drzewa"), Vector3(0,0,0), Vector3(0,0,0), Vector3(0.5,0.5,0.5), "drzewa.ms3d", "perPixelLightingAlpha", true));
        gameObjectManager:FindObject(skocznia_drzewa):SetShaderProgramParameter("lightPosition", Vector3(-50, 50, -50));


        skocznia_fizyka = gameObjectManager:AddObject(PhysicalGameObject(Hash("skocznia_fizyka"), Vector3(0,0,0), Vector3(0,0,0), Vector3(0.5,0.5,0.5), "skocznia_fizyka.ms3d", "perPixelLightingAlpha", true));
        gameObjectManager:FindObject(skocznia_fizyka):SetVisible(false);


        nartaLewa = gameObjectManager:AddObject(RenderableGameObject(Hash("lewaNarta"), Vector3(-0.1,4.5,-5), Vector3(0,0,0), Vector3(0.5, 0.5, 0.5), "narta.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(nartaLewa):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));
        narty.lewa.object = gameObjectManager:FindObject(nartaLewa);

        nartaPrawa = gameObjectManager:AddObject(RenderableGameObject(Hash("prawaNarta"), Vector3(0.0,4.5,-5), Vector3(0,0,0), Vector3(0.5, 0.5, 0.5), "narta.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(nartaPrawa):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));
        narty.prawa.object = gameObjectManager:FindObject(nartaPrawa);


        percent=0.8;
        drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading geometry");

        skybox = gameObjectManager:AddObject(RenderableGameObject(Hash("skybox"), Vector3(0,30,0), Vector3(0,0,0), Vector3(15, 15, 15), "skybox.ms3d", "textured_3d", false));
        renderer:DrawString(10, 10, "Loading skybox");
        percent=1;
        drawProgress(loaderScreen, progressBar, textureRegion, percent);
        camera = renderer:GetCamera();
        camera:SetPosition(Vector3(0, 5, -11));
end



update = function(dt)
    if keysState:IsKeyJustPressed(Input.KEY_CENTER) then
        audioSystem:PlaySound(sound, 1.0);
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
            camera:MoveLeft(-1*dt);
    elseif keysState:IsKeyPressed(Input.KEY_D) then
            camera:MoveLeft(1*dt);
    elseif keysState:IsKeyJustPressed(Input.KEY_R) then
            narty.lewa.object:SetPosition(Vector3(-0.1,4.5,-10));
            narty.prawa.object:SetPosition(Vector3(0.0,4.5,-10));
            narty.lewa.speed = Vector3(0,0,0);
            narty.prawa.speed = Vector3(0,0,0);
            katNart=0;
            leftRightAngle=0;
            narty.lewa.angle = 0;
            narty.prawa.angle = 0;
            narty.lewa.contact=false;
            narty.prawa.contact=false;
            inAir=false;
    end

    skoczniaObj = gameObjectManager:FindObject(nartaLewa);


    if keysState:IsKeyPressed(Input.KEY_SPACE) and inAir==false then
           narty.lewa.speed = narty.lewa.speed + Vector3(0, 2.2, 0);
           narty.prawa.speed = narty.prawa.speed + Vector3(0, 2.2, 0);
           inAir=true;
    end

    if keysState:IsKeyPressed(Input.KEY_M) then
        leftRightAngle=leftRightAngle+0.01;
-- katNart=katNart+0.01;
    elseif keysState:IsKeyPressed(Input.KEY_N) then
        --    katNart=katNart-0.01;
        leftRightAngle=leftRightAngle-0.01;
    end

    if keysState:IsKeyPressed(Input.KEY_K) then
         katNart=katNart+0.01;
    elseif keysState:IsKeyPressed(Input.KEY_L) then
        katNart=katNart-0.01;
    end

    newSkyboxPosition = camera:GetPosition();
    newSkyboxPosition = newSkyboxPosition+Vector3(0,10,0);
    gameObjectManager:FindObject(skybox):SetPosition(newSkyboxPosition);


    hor = camera:GetHorizontalAngle();
    ver = camera:GetVerticalAngle();

  --  gameObjectManager:FindObject(nartaLewa):SetPosition(camera:GetPosition() + Vector3(-0.045, -0.5, -0.1));

  --  gameObjectManager:FindObject(nartaPrawa):SetPosition(camera:GetPosition() + Vector3(0.045, -0.5, -0.1));
   -- gameObjectManager:FindObject(nartaPrawa):SetOrientation(Vector3(-katNart, katNart/3, 0));

    if keysState:IsKeyJustPressed(Input.KEY_P) then
        audioSystem:PlayMusic("/sdcard/music.mp3", 1.0);
    end

    update_ski(dt);


	--object = gameObjectManager:FindObject(gameObject1Id);
	--object:SetPosition(Vector3(1,1,0)*math.sin(angle/20));

--    translation = Matrix4x4();
 --   translation:SetTranslation(Vector3(-2,-1,-5));

  --  rotation = Matrix4x4();
  --  rotation:SetScale(Vector3(2.0, 2.0, 2.0));

  --  world = translation*rotation;

--	shaderParams = ShaderParametersList();
--	shaderParams:Add("lightPosition", Vector3(15*math.sin(angle/10), 10, 10));
 --   renderer:DrawGeometry(dwarfModel, world, shaderProgram, shaderParams);


--	translation:SetTranslation(Vector3(2, -1, -5));

--	world = translation*rotation;

--	shaderParams = ShaderParametersList();
--	shaderParams:Add("lightPosition", Vector3(10, 15*math.sin(angle/10), 10));
 --   renderer:DrawGeometry(dwarfModel, world, shaderProgram, shaderParams);


    angle= angle + dt*30;
end
