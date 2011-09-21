touchState = Input.Get():GetTouch();
keysState = Input.Get():GetKeys();
audioSystem = IAudioSystem.Get();
renderer = IRenderer.Get();
contentManager = IContentManager.Get();

angle = 0.0;

texture = contentManager:GetTextureManager():GetTexture("logo.png");
textureRegion = TextureRegion(0, 0, 0.5, 0.75);
sound = IContentManager.Get():GetSoundManager():GetSound('/sdcard/violin.wav');


update = function(dt)
    if touchState:IsPointerJustDown(Input.POINTER_0) then
        audioSystem:PlaySound(sound, 1.0);
    end

    if keysState:IsKeyJustPressed(Input.KEY_M) then
        audioSystem:SetMusicVolume(1.0);
    end
    if keysState:IsKeyJustPressed(Input.KEY_N) then
        audioSystem:SetMusicVolume(0.3);
    end

    for x=20,780,40 do
       for y=20,460,40 do
           renderer:DrawSprite(x, y, 30, 30, textureRegion, texture, angle);
       end
    end

    angle= angle + dt;
end
