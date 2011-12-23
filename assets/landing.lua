updateState_Landing = function(dt)


    skis.left.velocity = skis.left.velocity - skis.left.velocity*dt*0.3;
    skis.right.velocity = skis.right.velocity - skis.left.velocity*dt*0.3;

    if inAir==true then
        camera:SetVerticalAngle(skis.right.angle-skiesXAngle);
    end

    renderer:DrawString(380,240, round(jumpDistance,1) .. " m", 255, 0, 0);

    if os.time() - restartTime > 5 then
        SendResultToServer(jumpDistance);
        resetScene();
    end
end
