updateState_BeforeStart = function(dt)
    while skis.left.contact==false do
        update_ski(0.1);
    end

    accelZ = accelState:GetRawAcceleration().z;

    if accelZ > 15 then
        state="Start";
        started=true;
        audioSystem:StopSoundLoop();
        audioSystem:PlaySoundLoop(sounds.loops[2], 0.5);
    end
end
