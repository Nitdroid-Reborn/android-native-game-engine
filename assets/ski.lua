skiesXAngle=-0.30;
skiesZAngle=0.0;

inAir=false;

gravity = Vector3(0, -0.9, 0);
gravityDir = gravity:GetNormalized();

--structure describing skis
skis = {
         left = { object = 0, velocity = Vector3(), angle = 0, contactPoint = Vector3(), contactPointNormal = Vector3(0, 1,0), contact=false },
         right = {object = 0, velocity = Vector3(), angle = 0, contactPoint = Vector3(), contactPointNormal = Vector3(0,1,0), contact=false }
        }


update_ski = function(dt)
    delta = gravity;

    --if we are in air we set angles of skis to 0, there only used when skis ride down the ramp
    if inAir==true then
        skis.left.angle=0;
        skis.right.angle=0;
    end

    ----------------LFET SKI----------------------------

    --if ski is touching ground
    if skis.left.contact==true then
        --calculate result of gravity and reaction force from ground
        n = (skis.left.contactPointNormal)*-1*skis.left.contactPointNormal:DotProduct((gravity));
        delta = gravity + n;
    end


    --update position and velocity
    skis.left.velocity = skis.left.velocity + delta*dt;
    skis.left.object:SetPosition(skis.left.object:GetPosition() + skis.left.velocity*dt);


    colliderObject = gameObjectManager:FindObject(rampPhysicsObj);
    normal = Vector3(0, 1, 0);
    right = Vector3(1, 0, 0);

    --check collision
    wasCollision=false;
    while colliderObject:Collide(skis.left.object:GetPosition(), skis.left.contactPoint, skis.left.contactPointNormal) do
        skis.left.object:SetPosition(skis.left.object:GetPosition() + skis.left.contactPointNormal*0.005);
        wasCollision=true;
    end

    --if there was a collision update orientation
    if wasCollision==true then
        skis.left.contact=true;
        skis.left.angle = normal:DotProduct(skis.left.contactPointNormal);
        sign = normal:CrossProduct(skis.left.contactPointNormal):DotProduct(right);
        if sign>0 then
            skis.left.angle = math.acos(skis.left.angle);
        else
            skis.left.angle = -math.acos(skis.left.angle);
        end

        --modify velocity according to collision
        n = (skis.left.contactPointNormal)*-1*skis.left.contactPointNormal:DotProduct((skis.left.velocity));
        skis.left.velocity = skis.left.velocity + n;

        --set new orientation of 3d model
        skis.left.object:SetOrientation(Vector3(skis.left.angle, 0, 0));
        --if we were in air
        if inAir==true then
            inAir=false;
            Log("Position z: " .. skis.left.object:GetPosition().z*7);
        end
    else
        skis.left.contact=false;
        if inAir==true then
            --if we are in air and there was no collision set model orientation according to accelerometer
            skis.left.object:SetOrientation(Vector3(skiesXAngle, skiesXAngle, skiesZAngle));
        end
    end

    --------------------------------RIGHT SKI--------------------------------
    --update position

    delta = gravity;
    --if ski is touching ground
    if skis.right.contact==true then
        --calculate result of gravity and reaction force from ground
        n = (skis.right.contactPointNormal)*-1*skis.right.contactPointNormal:DotProduct((gravity));

        delta = delta + n;

    end


    --update position and velocity
    skis.right.velocity = skis.right.velocity + delta*dt;
    skis.right.object:SetPosition(skis.right.object:GetPosition() + skis.right.velocity*dt);


    --check collision
    wasCollision=false;
    while colliderObject:Collide(skis.right.object:GetPosition(), skis.right.contactPoint, skis.right.contactPointNormal) do
        skis.right.object:SetPosition(skis.right.object:GetPosition() + skis.right.contactPointNormal*0.005);
        wasCollision=true;
    end

    --if there was a collision update orientation
    if wasCollision==true then
        skis.right.contact=true;
        skis.right.angle = normal:DotProduct(skis.right.contactPointNormal);
        sign = normal:CrossProduct(skis.right.contactPointNormal):DotProduct(right);
        if sign>0 then
            skis.right.angle = math.acos(skis.right.angle);
        else
            skis.right.angle = -math.acos(skis.right.angle);
        end

        --modify velocity according to collision
        n = (skis.right.contactPointNormal)*-1*skis.right.contactPointNormal:DotProduct((skis.right.velocity));
        skis.right.velocity = skis.right.velocity + n;

        --set new orientation of 3d model
        skis.right.object:SetOrientation(Vector3(skis.right.angle, 0, 0));

        --if we were in air
        if inAir==true and collisionDelay==0 then
            inAir=false;
        end
    else
        skis.right.contact=false;
        if inAir==true then
            --if we are in air and there was no collision set model orientation according to accelerometer
            skis.right.object:SetOrientation(Vector3(skiesXAngle, -skiesXAngle, skiesZAngle));
        end
    end


    --update camera position
    camera:SetPosition((skis.right.object:GetPosition() + skis.left.object:GetPosition())/2 + skis.right.contactPointNormal*0.2);
    camera:SetVerticalAngle(skis.right.angle);

    if inAir==true then
        camera:SetVerticalAngle(skis.right.angle-skiesXAngle);
    end
end
