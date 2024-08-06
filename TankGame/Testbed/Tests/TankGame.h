#ifndef TANK_GAME_H
#define TANK_GAME_H

#include "../Framework/Test.h"
#include <list>

#define MAX_PROJECTILE 5

enum eTankCommand
{
    eTankCmd_MoveForward,
    eTankCmd_MoveBackWard,
    eTankCmd_RotateGunUp,
    eTankCmd_RotateGunDown,
    eTankCmd_ChargeGun,
    eTankCmd_FireGun,
    eTankCmd_Stop,
};

enum ProjectileState
{
    Ready,
    Fired
};

class Projectile
{
public:
    Projectile();
    Projectile(b2Vec2 position, b2Vec2 initialSpeed);
    void Update( float deltaTime );
    void Render( DebugDraw* drawInterface );
    ProjectileState GetState();

protected: 

    float m_lifetime = 3;
    b2Vec2 m_speed;
    b2Vec2 m_position;
    ProjectileState m_state = Ready;
    b2Color m_color;
};


class TankGun
{
public:
    TankGun( );

    void Init( b2Vec2 pivot, float diameter, float length  );

    void RotateUp();
    void RotateDown();
    void Fire();

    void Render( DebugDraw* drawInterface, b2Vec2 base );

protected:

    b2Vec2 m_pivot;
    float m_impulse = 0;
    float m_angle = 0;
    float m_rotationSpeed = 5;

    b2Vec2  m_gunPoly[4];
};


class TankWheel
{
public:

    TankWheel();
    void Init( b2Vec2 pos, float radius );

    void Render( DebugDraw* drawInterface, b2Vec2 base );
    void Update( float deltaTime);

    void SetSpeed( float speed );

protected:

    b2Vec2 m_position;
    float m_radius;

    float m_angle;
    float m_rotationSpeed;

};

class Tank
{
public:
    float const MAX_ACCELERATION = 5.0;
    float const MAX_SPEED = 10.0;
    float m_acceleration = 0.0;
    float m_speed = 0.0;

    Tank();

    void SetPosition( b2Vec2 pos );
    b2Vec2 GetPosition();
    void SetAcceleration(float acceleration);
    void SetSpeed(float speed);

    void ProcessCmd( eTankCommand cmd );
    void StopMovement(float deltaTime);
    void Render( DebugDraw* drawInterface );

    void Update( float deltaTime );

    Projectile FireGun();


protected:
    void Move(float deltaTime);

    b2Vec2 m_position;

    TankGun   m_gun;
    TankWheel m_wheels[4];

    float m_halfLength;
    b2Vec2 m_tankPoly[8];
};

class TankGame : Test 
{
public:

    TankGame();

    virtual void Step(Settings* settings);
	virtual void Keyboard(unsigned char key);
	virtual void KeyboardUp(unsigned char key);

    void UpdateProjectiles( float deltatime );
    void RenderProjectiles( );

    void RenderGround();
    void CheckGroundLimit();

    void FireGun();

    static Test* Create()
	{
		return new TankGame;
	}

protected:

    Tank m_tank;

    std::list<Projectile> m_projectiles;

    int m_nextProjectile = 0;

    //Ground
    b2Vec2 m_groundStart;
    b2Vec2 m_groundEnd;
};

#endif