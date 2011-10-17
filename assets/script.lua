touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
audioSystem = AudioSystem.Get();
renderer = Renderer.Get();
contentManager = ContentManager.Get();

angle = 0.0;

texture = contentManager:GetTextureManager():GetTexture(":logo.png");
textureRegion = TextureRegion(0, 0, 0.5, 0.75);
sound = ContentManager.Get():GetSoundManager():GetSound('/sdcard/violin.wav');


vector = Vector3(1, 0, 0);
vector2 = Vector3(0, 1, 0);

sum = vector + vector2;
mul = vector*5;
dot = vector*vector2;
cross = vector:CrossProduct(vector2)


Log(string.format("Sum: %f %f %f, Mul %f %f %f, dot %f %f %f, cross %f %f %f", sum.x, sum.y, sum.z,  mul.x, mul.y, mul.z, dot.x, dot.y, dot.z, cross.x, cross.y, cross.z));


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
        angle = angle + 5;
    end

    if keysState:IsKeyPressed(Input.KEY_RIGHT) then
        angle = angle - 5;
    end


    if keysState:IsKeyJustPressed(Input.KEY_P) then
        audioSystem:PlayMusic("/sdcard/music.mp3", 1.0);
    end

   -- for x=30,800,40 do
    --   for y=30,450,40 do
     --      renderer:DrawSprite(x, y, Renderer.NORMAL_LAYER, 30, 30, textureRegion, texture, angle);
      -- end
   -- end

    angle= angle + dt/3;
end
