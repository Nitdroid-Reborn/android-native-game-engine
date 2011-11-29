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
katNart=-0.15;

speed=Vector3();
inAir=false;

gravity = Vector3(0, -1.9, 0);
gravityDir = gravity:GetNormalized();


narty = {
         lewa = { object = 0, speed = Vector3(), angle = 0, contactPoint = Vector3(), contactPointNormal = Vector3(0, 1,0), contact=false },
         prawa = {object = 0, speed = Vector3(), angle = 0, contactPoint = Vector3(), contactPointNormal = Vector3(0,1,0), contact=false }
        }

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


        nartaLewa = gameObjectManager:AddObject(RenderableGameObject(Hash("lewaNarta"), Vector3(-0.1,4.5,-10), Vector3(0,0,0), Vector3(0.5, 0.5, 0.5), "narta.ms3d", "perPixelLighting", false));
        gameObjectManager:FindObject(nartaLewa):SetShaderProgramParameter("lightPosition", Vector3(-50, 250, -50));
        narty.lewa.object = gameObjectManager:FindObject(nartaLewa);

        nartaPrawa = gameObjectManager:AddObject(RenderableGameObject(Hash("prawaNarta"), Vector3(0.0,4.5,-10), Vector3(0,0,0), Vector3(0.5, 0.5, 0.5), "narta.ms3d", "perPixelLighting", false));
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




update_ski = function(dt)
    delta = gravity;

    if narty.lewa.contact==true then
        n = (narty.lewa.contactPointNormal)*-1*narty.lewa.contactPointNormal:DotProduct((gravity));
        delta = delta + n;
    end

    --update position
    narty.lewa.speed = narty.lewa.speed + delta*dt;
    narty.lewa.object:SetPosition(narty.lewa.object:GetPosition() + narty.lewa.speed*dt);

    colliderObject = gameObjectManager:FindObject(skocznia_fizyka);
    normal = Vector3(0, 1, 0);
    right = Vector3(1, 0, 0);

    --check collision
    wasCollision=false;
    while colliderObject:Collide(narty.lewa.object:GetPosition(), narty.lewa.contactPoint, narty.lewa.contactPointNormal) do
        narty.lewa.object:SetPosition(narty.lewa.object:GetPosition() + narty.lewa.contactPointNormal*0.005);
        wasCollision=true;
    end

    --if there was a collision update orientation
    if wasCollision==true then
        narty.lewa.contact=true;
        narty.lewa.angle = normal:DotProduct(narty.lewa.contactPointNormal);
        sign = normal:CrossProduct(narty.lewa.contactPointNormal):DotProduct(right);
        if sign>0 then
            narty.lewa.angle = math.acos(narty.lewa.angle);
        else
            narty.lewa.angle = -math.acos(narty.lewa.angle);
        end
        narty.lewa.object:SetOrientation(Vector3(narty.lewa.angle, 0, 0));
        if inAir==true then
            inAir=false;
        end

    else
        narty.lewa.contact=false;
        if inAir==true then
            narty.lewa.object:SetOrientation(Vector3(0, 2*katNart, 0));
        end
    end


    --update position
    delta = gravity;

    if narty.prawa.contact==true then
        n = (narty.prawa.contactPointNormal)*-1*narty.prawa.contactPointNormal:DotProduct((gravity));
        delta = delta + n;
    end

    narty.prawa.speed = narty.prawa.speed + delta*dt;
    narty.prawa.object:SetPosition(narty.prawa.object:GetPosition() + narty.prawa.speed*dt);


    --check collision
    wasCollision=false;
    while colliderObject:Collide(narty.prawa.object:GetPosition(), narty.prawa.contactPoint, narty.prawa.contactPointNormal) do
        narty.prawa.object:SetPosition(narty.prawa.object:GetPosition() + narty.prawa.contactPointNormal*0.005);
        wasCollision=true;
    end

    --if there was a collision update orientation
    if wasCollision==true then
        narty.prawa.contact=true;
        narty.prawa.angle = normal:DotProduct(narty.prawa.contactPointNormal);
        sign = normal:CrossProduct(narty.prawa.contactPointNormal):DotProduct(right);
        if sign>0 then
            narty.prawa.angle = math.acos(narty.prawa.angle);
        else
            narty.prawa.angle = -math.acos(narty.prawa.angle);
        end
        narty.prawa.object:SetOrientation(Vector3(narty.prawa.angle, 0, 0));
        if inAir==true then
            inAir=false;
        end
    else
        narty.prawa.contact=false;
        if inAir==true then
            narty.prawa.object:SetOrientation(Vector3(0, -2*katNart, 0));
        end
    end

    camera:SetPosition((narty.prawa.object:GetPosition() + narty.lewa.object:GetPosition())/2 + narty.prawa.contactPointNormal*0.2);
    camera:SetVerticalAngle(narty.prawa.angle);
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
    end

    skoczniaObj = gameObjectManager:FindObject(nartaLewa);
    if keysState:IsKeyPressed(Input.KEY_I) then
            skoczniaObj:SetPosition(skoczniaObj:GetPosition() + Vector3(0,-0.01, 0));
    elseif keysState:IsKeyPressed(Input.KEY_K) then
            skoczniaObj:SetPosition(skoczniaObj:GetPosition() + Vector3(0,0.01,0.0));
    elseif keysState:IsKeyPressed(Input.KEY_J) then
            skoczniaObj:SetPosition(skoczniaObj:GetPosition() + Vector3(0,0,0.01));
    elseif keysState:IsKeyPressed(Input.KEY_L) then
            skoczniaObj:SetPosition(skoczniaObj:GetPosition() + Vector3(0,0,-0.01));
    end


    if keysState:IsKeyPressed(Input.KEY_SPACE) and inAir==false then
            narty.lewa.speed = narty.lewa.speed + Vector3(0, 2.2, 0);
            narty.prawa.speed = narty.prawa.speed + Vector3(0, 2.2, 0);
            inAir=true;
    end

    if keysState:IsKeyPressed(Input.KEY_M) then
            katNart=katNart+0.1;
    elseif keysState:IsKeyPressed(Input.KEY_N) then
            katNart=katNart-0.1;
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
