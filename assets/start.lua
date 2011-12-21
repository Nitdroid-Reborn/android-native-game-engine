updateState_Start = function(dt)
    zPos = skis.left.object:GetPosition().z;
    accelZ = accelState:GetRawAcceleration().z;

    if accelZ > 15 and inAir==false and zPos>-4 and zPos<0.5 then
        --add some velocity in y axis
        skis.left.velocity = skis.left.velocity + Vector3(0, 0.66, 0);
        skis.right.velocity = skis.right.velocity + Vector3(0, 0.66, 0);
        --set that player is flying and unset any posible skis contact flags
        inAir=true;

        state="InAir";
        skis.left.contact=false;
        skis.right.contact=false;

        if zPos>-2  and zPos<0 then
            soundIndex = math.random(1,4);
            audioSystem:PlaySound(sounds.start.good[soundIndex], 1.0);
        else
            soundIndex = math.random(1,3);
            audioSystem:PlaySound(sounds.start.bad[soundIndex], 1.0);
        end
        startDelay=20;
    end

    if zPos>0.5 and inAir==false then
       -- skis.left.velocity = skis.left.velocity + Vector3(0, 0.0, 0);
       -- skis.right.velocity = skis.right.velocity + Vector3(0, 0.0, 0);
        --set that player is flying and unset any posible skis contact flags
        inAir=true;

        state="InAir";
        skis.left.contact=false;
        skis.right.contact=false;

        soundIndex = math.random(1,3);
        audioSystem:PlaySound(sounds.start.bad[soundIndex], 1.0);
    end
end
