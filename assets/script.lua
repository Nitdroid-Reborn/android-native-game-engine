--engine subsystems
touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
accelState = Input.Get():GetAccel();
audioSystem = AudioSystem.Get();
renderer = Renderer.Get();
contentManager = ContentManager.Get();
gameObjectManager = GameObjectManager.Get();

--handlers to game objects
rampObj=0;
treesObj=0;
rampPhysicsObj=0;
leftSkiObj=0;
rightSkiObj=0;
skyboxObj=0;

--include script source
scr = ScriptSource();
scr:Load(":ski.lua");
assert(loadstring(scr:GetSource()))()
scr:Dispose();

scr = ScriptSource();
scr:Load(":inair.lua");
assert(loadstring(scr:GetSource()))()
scr:Dispose();

scr = ScriptSource();
scr:Load(":start.lua");
assert(loadstring(scr:GetSource()))()
scr:Dispose();


startDelay=0;

jumpSound=0;
landingSound=0;

math.randomseed( os.time() );

sounds = {
            start = {
                        bad = {},
                        good = {}
                    },
            landing = {
                         short = {},
                         normal = {},
                         long = {}
                       }
         }

collisionDelay=0;


state = "Start"

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
        Log("du[a");

	loaderScreen = contentManager:GetTextureManager():GetTexture(":loaderScreen.png");
	progressBar = contentManager:GetTextureManager():GetTexture(":progressBar.png");
	textureRegion = TextureRegion(0.0, 0.0, 1.0, 1.0);


	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading...");

        sounds.start.bad[1] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/bad1.wav');
        sounds.start.bad[2] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/bad2.wav');
        sounds.start.bad[3] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/bad3.wav');
        sounds.start.good[1] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/ok1.wav');
        sounds.start.good[2] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/ok2.wav');
        sounds.start.good[3] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/ok3.wav');
        sounds.start.good[4] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/ok4.wav');

        sounds.landing.short[1] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/short1.wav');
        sounds.landing.short[2] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/short2.wav');
        sounds.landing.short[3] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/short3.wav');
        sounds.landing.normal[1] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/normal1.wav');
        sounds.landing.normal[2] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/normal2.wav');
        sounds.landing.normal[3] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/normal3.wav');
        sounds.landing.long[1] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/long1.wav');
        sounds.landing.long[2] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/long2.wav');
        sounds.landing.long[3] = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/long3.wav');


        jumpSound = ContentManager.Get():GetSoundManager():GetSound('dzwieki/najazd/ok1.wav');
        landingSound = ContentManager.Get():GetSoundManager():GetSound('dzwieki/skok/long1.wav');
	percent=0.2;

	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading sounds");

        --load and compile shaders
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



        --loads ramp 3d model and set shader params
        rampObj = gameObjectManager:AddObject(RenderableGameObject(Hash("ramp"), Vector3(0,0,0), Vector3(0,0,0), Vector3(0.5,0.5,0.5), "skocznia_teksury.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(rampObj):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));

        --loads trees model and set shader params
        treesObj = gameObjectManager:AddObject(RenderableGameObject(Hash("trees"), Vector3(0,0,0), Vector3(0,0,0), Vector3(0.5,0.5,0.5), "drzewa.ms3d", "perPixelLightingAlpha", true));
        gameObjectManager:FindObject(treesObj):SetShaderProgramParameter("lightPosition", Vector3(-50, 50, -50));

        --loads ramp physics model
        rampPhysicsObj = gameObjectManager:AddObject(PhysicalGameObject(Hash("ramp_physics"), Vector3(0,0,0), Vector3(0,0,0), Vector3(0.5,0.5,0.5), "skocznia_fizyka.ms3d", "perPixelLightingAlpha", true));
        gameObjectManager:FindObject(rampPhysicsObj):SetVisible(false);

        --loads ski model
        leftSkiObj = gameObjectManager:AddObject(RenderableGameObject(Hash("leftSki"), Vector3(-0.1,4.5,-10), Vector3(0,0,0), Vector3(0.5, 0.5, 0.5), "narta.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(leftSkiObj):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));
        skis.left.object = gameObjectManager:FindObject(leftSkiObj);

        rightSkiObj = gameObjectManager:AddObject(RenderableGameObject(Hash("rightSki"), Vector3(0.0,4.5,-10), Vector3(0,0,0), Vector3(0.5, 0.5, 0.5), "narta.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(rightSkiObj):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));
        skis.right.object = gameObjectManager:FindObject(rightSkiObj);


	percent=0.8;
	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        renderer:DrawString(10, 10, "Loading geometry");

        --loads skybox
        skyboxObj = gameObjectManager:AddObject(RenderableGameObject(Hash("skybox"), Vector3(0,30,0), Vector3(0,0,0), Vector3(15, 15, 15), "skybox.ms3d", "textured_3d", false));
        renderer:DrawString(10, 10, "Loading skybox");
	percent=1;
	drawProgress(loaderScreen, progressBar, textureRegion, percent);
        camera = renderer:GetCamera();
        camera:SetPosition(Vector3(0, 5, -11));
end


update = function(dt)
--    if keysState:IsKeyJustPressed(Input.KEY_M) then
--        audioSystem:SetMusicVolume(1.0);
--    end
--    if keysState:IsKeyJustPressed(Input.KEY_N) then
--        audioSystem:SetMusicVolume(0.3);
--    end

--    if keysState:IsKeyPressed(Input.KEY_CENTER) then
--        angle = 0;
--    end
    --get camera object
    camera = renderer:GetCamera();

    --check and interpret input
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
    elseif keysState:IsKeyJustPressed(Input.KEY_CENTER) then
            --resets scene
            skis.left.object:SetPosition(Vector3(-0.1,4.5,-10));
            skis.right.object:SetPosition(Vector3(0.0,4.5,-10));
            skis.left.velocity = Vector3(0,0,0);
            skis.right.velocity = Vector3(0,0,0);
            skiesXAngle=0;
            skiesZAngle=0;
            skis.left.angle = 0;
            skis.right.angle = 0;
            skis.left.contact=false;
            skis.right.contact=false;
            inAir=false;
            state="Start";
    end

    --moves skybox to stay in center of view
    newSkyboxPosition = camera:GetPosition();
    newSkyboxPosition = newSkyboxPosition+Vector3(0,30,0);
    gameObjectManager:FindObject(skyboxObj):SetPosition(newSkyboxPosition);


    if keysState:IsKeyJustPressed(Input.KEY_P) then
        audioSystem:PlayMusic("/sdcard/music.mp3", 1.0);
    end

    --if player is fyling
    if state=="Start" then
        updateState_Start(dt);
    elseif state=="InAir" then
        updateState_InAir(dt);
    end

    --check for collision and landing
    update_ski(dt);

end
