#include "TankGame.h"
#include <ctype.h>
#include <iostream>
#include <glui/glui_internal.h>


Projectile::Projectile()
{
    m_position = b2Vec2_zero;
    m_position.y += 10;
    m_speed = b2Vec2_zero;
    m_color.Set(255, 0, 0);
}

Projectile::Projectile(b2Vec2 position, b2Vec2 initialSpeed)
{
    m_position = position;
    m_speed = initialSpeed;
    m_color.Set(255, 0, 0);
}

void Projectile::Update( float deltaTime )
{
    if (m_state == Fired) { return; }
    m_lifetime -= deltaTime;
    m_state = (m_lifetime <= 0) ? Fired : Ready;

    float gravityAcceleration = -9.8;
    float speedY = m_speed.y;
    speedY += gravityAcceleration * deltaTime;
    m_speed.y = speedY;
    m_position += m_speed * deltaTime;
}

void Projectile::Render( DebugDraw* drawInterface )
{
    if (m_state == Fired) { return; }
    drawInterface->DrawPoint(m_position, 3, m_color);
}

ProjectileState Projectile::GetState()
{
    return m_state;
}

TankGun::TankGun( )
{
    m_pivot.SetZero();
    m_impulse = 0.0f;
    m_angle = 0.0f;
}

void TankGun::Init( b2Vec2 pivot, float diameter, float length  )
{
    m_pivot = pivot;

    float radius = diameter / 2.0f;
    m_gunPoly[0] = b2Vec2( 0.0f, -radius );
    m_gunPoly[1] = b2Vec2( length, -radius );
    m_gunPoly[2] = b2Vec2( length, radius );
    m_gunPoly[3] = b2Vec2( 0.0f, radius );
}

void TankGun::RotateUp()
{
    m_angle += DEG2RAD(m_rotationSpeed);
    float currentAngleDeg = RAD2DEG(m_angle);
    if (currentAngleDeg > 180) { 
        m_angle = DEG2RAD(180);
    }
}

void TankGun::RotateDown()
{
    m_angle -= DEG2RAD(m_rotationSpeed);
    float currentAngleDeg = RAD2DEG(m_angle);
    if (currentAngleDeg < 0) { 
        m_angle = DEG2RAD(0); 
    }
}

Projectile TankGun::Fire(b2Vec2 position)
{
    b2Vec2 m_projectileSpeed = b2Vec2_zero;
    m_projectileSpeed.Set(cosf(m_angle) * 10, sinf(m_angle) * 10);

    Projectile m_projectile = Projectile(position + m_pivot, m_projectileSpeed);
    return m_projectile;
}

void TankGun::Render( DebugDraw* drawInterface, b2Vec2 base )
{
    b2Transform gunTransform( base + m_pivot, b2Rot( m_angle ) );

    //Draw gun
    b2Vec2  renderPoly[4];
    for(int i=0;i<4;i++)
    {
        renderPoly[i] = b2Mul( gunTransform, m_gunPoly[i] );
    }

    drawInterface->DrawSolidPolygon( renderPoly, 4 , b2Color(0.0,1.0,0.0) );
}

//************************WHEELS*****************************
TankWheel::TankWheel()
{
    m_position.SetZero();
    m_radius = 0.0f;
    m_angle = 0.0f;
    m_rotationSpeed = 0.0f;
}

void TankWheel::Init( b2Vec2 pos, float radius )
{
    m_position = pos;
    m_radius = radius;
}

void TankWheel::Render( DebugDraw* drawInterface, b2Vec2 base )
{
    if( !drawInterface )
        return;

    b2Vec2 dir;
    dir.Set( cosf(m_angle), sinf(m_angle) );

    drawInterface->DrawSolidCircle( base + m_position, m_radius, dir, b2Color(0.0,1.0,0.0) ); 
}

void TankWheel::Update(float deltaTime)
{
    m_angle += m_rotationSpeed * deltaTime;
}

void TankWheel::SetSpeed(float speed)
{
    float m_rotationAngleRAD = -speed / m_radius;
    m_rotationSpeed = m_rotationAngleRAD;
}


//******************************TANK**********************************

Tank::Tank()
{
    m_position.SetZero();
    m_halfLength = 1.0f;
    
    float wheelRadius = 0.2f;

    m_wheels[0].Init( b2Vec2( -0.75f, wheelRadius ), wheelRadius );
    m_wheels[1].Init( b2Vec2( -0.25f, wheelRadius ), wheelRadius );;
    m_wheels[2].Init( b2Vec2( 0.25f, wheelRadius ), wheelRadius );;
    m_wheels[3].Init( b2Vec2( 0.75f, wheelRadius ), wheelRadius );;

    m_tankPoly[0] = b2Vec2(-1.0f,0.0f);
    m_tankPoly[1] = b2Vec2(1.0f,0.0f);
    m_tankPoly[2] = b2Vec2(1.0f,0.4f);
    m_tankPoly[3] = b2Vec2(0.6f,0.4f);
    m_tankPoly[4] = b2Vec2(0.4f,0.8f);
    m_tankPoly[5] = b2Vec2(-0.4f,0.8f);
    m_tankPoly[6] = b2Vec2(-0.6f,0.4f);
    m_tankPoly[7] = b2Vec2(-1.0f,0.4f);

    //Raise body
    for(int i=0;i<8;i++)
    {
        m_tankPoly[i] += b2Vec2( 0.0f, 2.0f * wheelRadius );
    }

    //Init gun size and position
    m_gun.Init( b2Vec2( 0.0f, 0.6 + 2.0f * wheelRadius) , 0.15f, 1.5f);
}

void Tank::SetPosition( b2Vec2 pos )
{
    m_position = pos;
}

b2Vec2 Tank::GetPosition()
{
    return m_position;
}

void Tank::SetAcceleration(float acceleration)
{
    m_acceleration = acceleration;
}

void Tank::SetSpeed(float speed)
{
    m_speed = speed;

    if (m_speed >= MAX_SPEED || m_speed <= -MAX_SPEED) {
        float direction = m_acceleration / abs(m_acceleration);
        m_speed = direction * MAX_SPEED;
    }

    for (int i = 0; i < 4; i++)
    {
        m_wheels[i].SetSpeed(m_speed);
    }
}

void Tank::ProcessCmd( eTankCommand cmd )
{
    switch (cmd)
    {
    case eTankCmd_MoveForward:
        m_acceleration = MAX_ACCELERATION;
        break;
    case eTankCmd_MoveBackWard:
        m_acceleration = -MAX_ACCELERATION;
        break;
    case eTankCmd_RotateGunUp:
        m_gun.RotateUp();
        break;
    case eTankCmd_RotateGunDown:
        m_gun.RotateDown();
        break;
    case eTankCmd_ChargeGun:
        break;
    case eTankCmd_FireGun:
        break;
    case eTankCmd_Stop:
        m_acceleration = 0;
        break;
    default:
        break;
    }
}

void Tank::StopMovement(float deltaTime)
{
    if (m_speed == 0) { return; }
    float tempSpeed = m_speed;
    tempSpeed -= tempSpeed * MAX_ACCELERATION * deltaTime;
    if (tempSpeed < 0.1 && tempSpeed > -0.1) { tempSpeed = 0; }

    SetSpeed(tempSpeed);
    m_position.x += m_speed * deltaTime;
    std::cout << "m_position: " << m_position.x << std::endl;
}

void Tank::Render( DebugDraw* drawInterface )
{
    if( !drawInterface )
        return;

    //Draw tank body
    b2Vec2  renderPoly[8];
    for(int i=0;i<8;i++)
    {
        renderPoly[i] = m_tankPoly[i];
        renderPoly[i] += m_position;
    }

    drawInterface->DrawPolygon( renderPoly, 8 , b2Color(0.0,1.0,0.0) );

    //Draw each tank wheel
    for( int i=0;i<4;i++)
    {
        m_wheels[i].Render( drawInterface, m_position );
    }

    m_gun.Render( drawInterface, m_position );

}

void Tank::Update( float deltaTime )
{
    Move(deltaTime);

    for (int i = 0; i < 4; i++)
    {
        m_wheels[i].Update(deltaTime);
    }
}

Projectile Tank::FireGun()
{
    Projectile m_projectile = m_gun.Fire(m_position);
    return m_projectile;
}

void Tank::Move(float deltaTime)
{
    if (m_acceleration == 0) {
        StopMovement(deltaTime);
        return;
    }

    float tempSpeed = m_speed;
    tempSpeed += m_acceleration * deltaTime;

    SetSpeed(tempSpeed);

    m_position.x += m_speed * deltaTime;
}


//************************************GAME**********************************

TankGame::TankGame()
:Test()
{
    m_groundStart = b2Vec2( -40.0f, 10.0f );
    m_groundEnd = b2Vec2( 40.0f, 10.0f );

    b2Vec2 tankInitialPosition = m_groundStart + m_groundEnd;
    tankInitialPosition *= 0.5f;

    m_tank.SetPosition(tankInitialPosition);
}

void TankGame::Step(Settings* settings)
{
    float dt = 1.0f / settings->hz;

    m_tank.Update( dt );

    UpdateProjectiles(dt);

    CheckGroundLimit();

    //Render game
    m_tank.Render( &m_debugDraw );
    RenderProjectiles();
    RenderGround();
}

void TankGame::Keyboard(unsigned char key)
{
    char commandKey = (char)tolower(key);

    switch (commandKey)
    {
    case 'a':
        m_tank.ProcessCmd(eTankCmd_MoveBackWard);
        break;
    case 'd':
        m_tank.ProcessCmd(eTankCmd_MoveForward);
        break;
    case 'q':
        m_tank.ProcessCmd(eTankCmd_RotateGunUp);
        break;
    case 'e':
        m_tank.ProcessCmd(eTankCmd_RotateGunDown);
        break;
    case 'w':
        FireGun();
    default:
        break;
    }
}

void TankGame::KeyboardUp(unsigned char key)
{
    char commandKey = (char)tolower(key);
    switch (commandKey)
    {
    case 'a':
    case 'd':
        m_tank.ProcessCmd(eTankCmd_Stop);
        break;
    default:
        break;
    }
    std::cout << commandKey << std::endl;
}

void TankGame::CheckGroundLimit()
{
    if (m_tank.GetPosition().x + 1 > m_groundEnd.x) {
        m_tank.SetPosition(b2Vec2(m_groundEnd.x - 1, m_groundEnd.y));
        m_tank.SetSpeed(0);
        m_tank.SetAcceleration(0);
    }
    else if (m_tank.GetPosition().x - 1 < m_groundStart.x) {
        m_tank.SetPosition(b2Vec2(m_groundStart.x + 1, m_groundStart.y));
        m_tank.SetSpeed(0);
        m_tank.SetAcceleration(0);
    }
}

void TankGame::FireGun()
{
    m_projectiles.push_back(m_tank.FireGun());
}

void TankGame::UpdateProjectiles( float deltatime )
{
    std::list<Projectile>::iterator projectilesIterator = m_projectiles.begin();

    while (projectilesIterator != m_projectiles.end()) {
        bool isActive = projectilesIterator->GetState() != Fired;

        if (isActive) {
            projectilesIterator->Update(deltatime);
            projectilesIterator++;
        }
        else {
            m_projectiles.erase(projectilesIterator++);
        }
    }
}


void TankGame::RenderProjectiles( )
{
    std::list<Projectile>::iterator projectilesIterator = m_projectiles.begin();
    for( int i=0;i< m_projectiles.size();i++)
    {
        projectilesIterator->Render( &m_debugDraw );
        projectilesIterator++;
    }
}

void TankGame::RenderGround()
{
    b2Color groundColor = b2Color( 0.0f, 0.0f, 1.0f );
    m_debugDraw.DrawSegment( m_groundStart, m_groundEnd, groundColor );

    m_debugDraw.DrawSegment( m_groundStart, m_groundStart + b2Vec2(0.0f,1.0f), groundColor );
    m_debugDraw.DrawSegment( m_groundEnd, m_groundEnd + b2Vec2(0.0f,1.0f), groundColor );
}