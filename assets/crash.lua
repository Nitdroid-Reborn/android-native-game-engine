updateState_Crash = function(dt)

    skis.left.velocity = skis.left.velocity - skis.left.velocity*dt*0.3;
    skis.right.velocity = skis.right.velocity - skis.left.velocity*dt*0.3;

    if os.time() - restartTime > 5 then
        resetScene();
    end
end
