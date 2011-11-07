touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
audioSystem = AudioSystem.Get();
renderer = Renderer.Get();
contentManager = ContentManager.Get();

angle = 0.0;

texture = contentManager:GetTextureManager():GetTexture("logo.png");
textureRegion = TextureRegion(0, 0, 0.5, 0.75);
sound = ContentManager.Get():GetSoundManager():GetSound('/sdcard/violin.wav');


shaderProgram = contentManager:GetShaderProgramManager():GetShaderProgram("perPixelLighting");
vertexShader = contentManager:GetShaderManager():GetShader("shaders/pixelLighting.vert");
fragmentShader = contentManager:GetShaderManager():GetShader("shaders/pixelLighting.frag");

shaderProgram:Get():AddShader(vertexShader);
shaderProgram:Get():AddShader(fragmentShader);
shaderProgram:Get():Link();

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

	camera = renderer:GetCamera();

	if keysState:IsKeyPressed(Input.KEY_UP) then
		camera:MoveForward(5*dt);
	elseif keysState:IsKeyPressed(Input.KEY_DOWN) then
		camera:MoveForward(-5*dt);
    elseif keysState:IsKeyPressed(Input.KEY_RIGHT) then
		camera:RotateLeft(1*dt);
    elseif keysState:IsKeyPressed(Input.KEY_LEFT) then
		camera:RotateLeft(-1*dt);
	elseif keysState:IsKeyPressed(Input.KEY_Z) then
		camera:MoveUp(2*dt);
	elseif keysState:IsKeyPressed(Input.KEY_X) then
		camera:MoveUp(-2*dt);
	elseif keysState:IsKeyPressed(Input.KEY_A) then
		camera:MoveLeft(-2*dt);
	elseif keysState:IsKeyPressed(Input.KEY_D) then
		camera:MoveLeft(2*dt);
	end


    if keysState:IsKeyJustPressed(Input.KEY_P) then
        audioSystem:PlayMusic("/sdcard/music.mp3", 1.0);
    end

    translation = Matrix4x4();
    translation:SetTranslation(Vector3(-2,-1,-5));

    rotation = Matrix4x4();
    rotation:SetScale(Vector3(2.0, 2.0, 2.0));

    world = translation*rotation;

	shaderParams = ShaderParametersList();
	shaderParams:Add("lightPosition", Vector3(15*math.sin(angle/10), 10, 10));
    renderer:DrawGeometry(dwarfModel, world, shaderProgram, shaderParams);


	translation:SetTranslation(Vector3(2, -1, -5));

	world = translation*rotation;

	shaderParams = ShaderParametersList();
	shaderParams:Add("lightPosition", Vector3(10, 15*math.sin(angle/10), 10));
    renderer:DrawGeometry(dwarfModel, world, shaderProgram, shaderParams);


    angle= angle + dt*30;
end
