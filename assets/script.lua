touchState = Input.Get():GetTouchState();
keysState = Input.Get():GetKeyState();
audioSystem = IAudioSystem.Get();
renderer = IRenderer.Get();
contentManager = IContentManager.Get();

angle = 0.0;

texture = contentManager:GetTextureManager():GetTexture("logo.png");
textureRegion = TextureRegion:new()
sound = IContentManager.Get():GetSoundManager():GetSound('/sdcard/violin.wav');


update = function(dt)
    if touchState:IsPointerJustDown(ENGINE_POINTER_0) then
        Logger.Log("ok down");

        audioSystem:PlaySound(sound, 1.0);
    end

    if keysState:IsKeyJustPressed(ENGINE_KEYCODE_M) then
        audioSystem:SetMusicVolume(1.0);
    end
    if keysState:IsKeyJustPressed(ENGINE_KEYCODE_N) then
        audioSystem:SetMusicVolume(0.5);
    end

    for x=20,780,40 do
        for y=20,460,40 do
            renderer:DrawTexturedSprite(x, y, 30, 30, textureRegion, texture, angle);
        end
    end

    angle= angle + dt;
end
