touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
audioSystem = AudioSystem.Get();
renderer = Renderer.Get();
contentManager = ContentManager.Get();

angle = 0.0;

texture = contentManager:GetTextureManager():GetTexture("logo.png");
textureRegion = TextureRegion(0, 0, 0.5, 0.75);
sound = ContentManager.Get():GetSoundManager():GetSound('/sdcard/violin.wav');

shaderProgram = ContentManager.Get():GetShaderProgramManager():GetShaderProgram("textured_3d");
dwarfModel = ContentManager.Get():GetModelGeometryManager():GetModelGeometry("krasnal.ms3d");


--vector = Vector3(1, 0, 0);
--vector2 = Vector3(0, 1, 0);

--sum = vector + vector2;
--mul = vector*5;
--dot = vector*vector2;
--cross = vector:CrossProduct(vector2)



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

    if keysState:IsKeyPressed(Input.KEY_LEFT) then
     --   angle = angle + 5;
    end

    if keysState:IsKeyPressed(Input.KEY_RIGHT) then
     --   angle = angle - 5;
    end


    if keysState:IsKeyJustPressed(Input.KEY_P) then
        audioSystem:PlayMusic("/sdcard/music.mp3", 1.0);
    end

    translation = Matrix4x4();
    translation:SetTranslation(Vector3(0,-1,-5));

    rotation = Matrix4x4();
    rotation:SetRotationY(angle);

    world = translation*rotation;
	
    renderer:DrawGeometry(dwarfModel, world, shaderProgram);

 translation = Matrix4x4();
    translation:SetTranslation(Vector3(5,-1,-5));

    rotation = Matrix4x4();
    rotation:SetRotationY(angle);

    world = translation*rotation;
	
    renderer:DrawGeometry(dwarfModel, world, shaderProgram);
    

    

    angle= angle + dt/10;
end
