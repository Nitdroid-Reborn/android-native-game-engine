updateState_InAir = function(dt)
        boost = Vector3(0,0.425,0);
        if startDelay == 0 then
            --read accelerometer state and calculate skis angle
            accel = accelState:GetAcceleration();
            --angle in z axis
            skiesZAngle = -round(accel.y,2)/500*180/3.1415;
            --clamp it to (-0.5;0.5) range
            if(skiesZAngle>0.5) then skiesZAngle=0.5; end
            if(skiesZAngle<-0.5) then skiesZAngle=-0.5; end

            --skis angle in x axis
            skiesXAngle = -1.2*(9-round(accel.z,2))/500*180/3.1415;
            --clam it to (-0.5;0)
            if(skiesXAngle<-0.5) then skiesXAngle=-0.5; end
            if(skiesXAngle>0.0) then skiesXAngle=0.0; end

            --boost vector directed opposite to gravity force


            --piecewise function of boost depening on skis x angle
            if skiesXAngle*skiesXAngle<0.05 then
                boost = Vector3(0, 8.5*skiesXAngle*skiesXAngle, 0);
            elseif skiesXAngle*skiesXAngle>0.2 then
                boost = Vector3(0, 2.125-8.5*skiesXAngle*skiesXAngle, 0);
            else
                boost = Vector3(0, 0.425, 0);
            end
            --if skis z angle is different than 0 it reduce boost
            boost.y = boost.y - skiesZAngle*skiesZAngle
            if boost.y < 0.0 then
                boost.y = 0.0;
            end
        end

        if startDelay > 0 then
            startDelay=startDelay-1;
        end


        --update skis velocities
        skis.left.velocity = skis.left.velocity + boost*dt;
        skis.right.velocity = skis.right.velocity + boost*dt;

        renderer:DrawSprite(400.0, 40.0, 10.0, 40.0, 40.0, (0.425-boost.y)*600, boost.y*600, 0, 255, 0.0);


        if skis.left.contact==true or skis.right.contact==true then
            zPos = skis.left.object:GetPosition().z*7;
            state="Landing";

            if zPos>120 then
                soundIndex = math.random(1,3);
                audioSystem:PlaySound(sounds.landing.long[soundIndex], 1.0);
            elseif zPos>100 then
                soundIndex = math.random(1,3);
                audioSystem:PlaySound(sounds.landing.normal[soundIndex], 1.0);
            else
                soundIndex = math.random(1,3);
                audioSystem:PlaySound(sounds.landing.short[soundIndex], 1.0);
            end
        end
end
