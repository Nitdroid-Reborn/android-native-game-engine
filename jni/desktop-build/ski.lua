katNart=-0.30;

leftRightAngle=0.0;

speed=Vector3();
inAir=false;

gravity = Vector3(0, -1.9, 0);
gravityDir = gravity:GetNormalized();


narty = {
         lewa = { object = 0, speed = Vector3(), angle = 0, contactPoint = Vector3(), contactPointNormal = Vector3(0, 1,0), contact=false },
         prawa = {object = 0, speed = Vector3(), angle = 0, contactPoint = Vector3(), contactPointNormal = Vector3(0,1,0), contact=false }
        }


update_ski = function(dt)
    delta = gravity;

    if inAir==true then
        narty.lewa.angle=0;
        narty.prawa.angle=0;
    end

    if narty.lewa.contact==true then
        n = (narty.lewa.contactPointNormal)*-1*narty.lewa.contactPointNormal:DotProduct((gravity));
        delta = gravity + n;
    end


    --update position
    narty.lewa.speed = narty.lewa.speed + delta*dt;


    narty.lewa.object:SetPosition(narty.lewa.object:GetPosition() + narty.lewa.speed*dt);


    colliderObject = gameObjectManager:FindObject(skocznia_fizyka);
    normal = Vector3(0, 1, 0);
    right = Vector3(1, 0, 0);

    --check collision
    wasCollision=false;
    while colliderObject:Collide(narty.lewa.object:GetPosition() + Vector3(0,-0.001,0), narty.lewa.contactPoint, narty.lewa.contactPointNormal) do
        narty.lewa.object:SetPosition(narty.lewa.object:GetPosition() + narty.lewa.contactPointNormal*0.005);
        wasCollision=true;
    end

    --if there was a collision update orientation
    if wasCollision==true then
        narty.lewa.contact=true;
        narty.lewa.angle = normal:DotProduct(narty.lewa.contactPointNormal);
        sign = normal:CrossProduct(narty.lewa.contactPointNormal):DotProduct(right);
        if sign>0 then
            narty.lewa.angle = math.acos(narty.lewa.angle);
        else
            narty.lewa.angle = -math.acos(narty.lewa.angle);
        end

        n = (narty.lewa.contactPointNormal)*-1*narty.lewa.contactPointNormal:DotProduct((narty.lewa.speed));
        narty.lewa.speed = narty.lewa.speed + n;


        narty.lewa.object:SetOrientation(Vector3(narty.lewa.angle, 0, 0));
        if inAir==true and collisionDelay==0 then
            inAir=false;
        end
    else
        narty.lewa.contact=false;
        if inAir==true then
            narty.lewa.object:SetOrientation(Vector3(katNart, katNart, leftRightAngle));
        end
    end


    --update position
    delta = gravity;

    if narty.prawa.contact==true then
        n = (narty.prawa.contactPointNormal)*-1*narty.prawa.contactPointNormal:DotProduct((gravity));

        delta = delta + n;

    end



    narty.prawa.speed = narty.prawa.speed + delta*dt;
    narty.prawa.object:SetPosition(narty.prawa.object:GetPosition() + narty.prawa.speed*dt);


    --check collision
    wasCollision=false;
    while colliderObject:Collide(narty.prawa.object:GetPosition() + Vector3(0,-0.001,0), narty.prawa.contactPoint, narty.prawa.contactPointNormal) do
        narty.prawa.object:SetPosition(narty.prawa.object:GetPosition() + narty.prawa.contactPointNormal*0.005);
        wasCollision=true;
    end

    --if there was a collision update orientation
    if wasCollision==true then
        narty.prawa.contact=true;
        narty.prawa.angle = normal:DotProduct(narty.prawa.contactPointNormal);
        sign = normal:CrossProduct(narty.prawa.contactPointNormal):DotProduct(right);
        if sign>0 then
            narty.prawa.angle = math.acos(narty.prawa.angle);
        else
            narty.prawa.angle = -math.acos(narty.prawa.angle);
        end

        n = (narty.prawa.contactPointNormal)*-1*narty.prawa.contactPointNormal:DotProduct((narty.prawa.speed));
        narty.prawa.speed = narty.prawa.speed + n;


        narty.prawa.object:SetOrientation(Vector3(narty.prawa.angle, 0, 0));
        if inAir==true and collisionDelay==0 then
            inAir=false;
        end
    else
        narty.prawa.contact=false;
        if inAir==true then
            narty.prawa.object:SetOrientation(Vector3(katNart, -katNart, leftRightAngle));
        end
    end



camera:SetPosition(Vector3(-5, -2, 5));
--camera:SetHorizontalAngle(0.5);

   camera:SetPosition((narty.prawa.object:GetPosition() + narty.lewa.object:GetPosition())/2 + narty.prawa.contactPointNormal*0.2);
   camera:SetVerticalAngle(narty.prawa.angle);

    if inAir==true then
        camera:SetVerticalAngle(narty.prawa.angle-katNart);
    end
end
