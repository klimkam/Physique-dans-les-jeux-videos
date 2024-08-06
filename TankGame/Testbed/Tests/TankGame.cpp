#include "TankGame.h"
#include <ctype.h>
#include <iostream>


void Projectile::Update( float deltaTime )
{

}

void Projectile::Render( DebugDraw* drawInterface )
{

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

}

void TankGun::RotateDown()
{

}

void TankGun::Fire( Projectile& projectile )
{

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

void TankWheel::Update( float deltaTime)
{

}

void TankWheel::SetSpeed( float speed )
{

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
        break;
    case eTankCmd_RotateGunDown:
        break;
    case eTankCmd_ChargeGun:
        break;
    case eTankCmd_FireGun:
        break;
    default:
        break;
    }
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
}

void Tank::Move(float deltaTime)
{
    m_speed += m_acceleration * deltaTime;
    
    if (m_speed >= MAX_SPEED || m_speed <= -MAX_SPEED) {
        float direction = m_acceleration / abs(m_acceleration);
        m_speed = direction * MAX_SPEED;
    }

    m_position.x += m_speed * deltaTime;
}

void Tank::RotateGun( )
{

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
    default:
        break;
    }
}

void TankGame::KeyboardUp(unsigned char key)
{
    char commandKey = (char)tolower(key);
}

void TankGame::CheckGroundLimit()
{

}

void TankGame::UpdateProjectiles( float deltatime )
{
    for( int i=0;i<MAX_PROJECTILE;i++)
    {
        m_projectiles[i].Update(deltatime);
    }
}


void TankGame::RenderProjectiles( )
{
    for( int i=0;i<MAX_PROJECTILE;i++)
    {
        m_projectiles[i].Render( &m_debugDraw );
    }
}

void TankGame::RenderGround()
{

    b2Color groundColor = b2Color( 0.0f, 0.0f, 1.0f );
    m_debugDraw.DrawSegment( m_groundStart, m_groundEnd, groundColor );

    m_debugDraw.DrawSegment( m_groundStart, m_groundStart + b2Vec2(0.0f,1.0f), groundColor );
    m_debugDraw.DrawSegment( m_groundEnd, m_groundEnd + b2Vec2(0.0f,1.0f), groundColor );

}