#include "CollisionBoxDemo.h"


//88888888888888888888888888888888888888888888888888888888888888888888888

void CollisionBoxDemo::KeyboardUp(unsigned char key)
{
	float impulseStrength = 100.0f;
	switch (key)
	{
		case 'n':
			CreateParticle();
			break;
	}
	
}

void CollisionBoxDemo::CheckWallCollision()
{
	b2Vec2 normalDown = b2Vec2(0.0, 1.0);
	b2Vec2 normalUp = b2Vec2(0.0, -1.0);
	b2Vec2 normalLeft = b2Vec2(1.0, 0.0);
	b2Vec2 normalRight = b2Vec2(-1.0, 0.0);


	// Collisions with Walls
	for (int i = 0; i < m_particles.size(); i++)
	{
		//Down Wall
		

		//Up Wall
		

		//Left Wall
		
		//Right Wall
		
	}
}
void CollisionBoxDemo::PreventWallOverlap(Particle& particleA, float penetrationDepth,b2Vec2 normal)
{
	//Prevent interpenetration
	
}

void CollisionBoxDemo::PreventParticleOverlap(Particle& particleA, Particle& particleB, float penetrationDepth)
{
	// On veut replacer les 2 parcule juste assez pour qu'il ne s'overlap plus selon la direction de leur contact qui est deduis de la direction calculer avec leur 2 position.
	
}

void CollisionBoxDemo::CheckParticleToParticleCollision()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		for (int j = i + 1; j < m_particles.size(); j++)
		{
			float distMin = m_particles[i].GetRadius() + m_particles[j].GetRadius();
			b2Vec2 dirParticles = (m_particles[i].GetPosition() - m_particles[j].GetPosition());
			float distance = dirParticles.Normalize();

			float penetrationDepth = (distMin - distance) * 0.5;
			if (distance <= distMin)
			{
				PreventParticleOverlap(m_particles[i], m_particles[j],penetrationDepth);
				ApplyCollisionBetweenParticles(m_particles[i], m_particles[j], dirParticles);
			}
		}
	}
	
}


void CollisionBoxDemo::DrawBoundingBox()
{
	b2Vec2 box[4];
	box[0].Set(m_boundingBoxMin.x, m_boundingBoxMin.y);
	box[1].Set(m_boundingBoxMin.x, m_boundingBoxMax.y);
	box[2].Set(m_boundingBoxMax.x, m_boundingBoxMax.y);
	box[3].Set(m_boundingBoxMax.x, m_boundingBoxMin.y);

	m_debugDraw.DrawPolygon(box, 4, b2Color(0.0f, 1.0f, 0.0f));
}

CollisionBoxDemo::CollisionBoxDemo()
{ 

	m_boundingBoxMin = b2Vec2(0.0f, 0.0f);
	m_boundingBoxMax = b2Vec2(100.0f, 100.0f);
	m_maxParticle = 10;
	CreateParticle();

}

void CollisionBoxDemo::ApplyCollisionWithStaticObject(Particle& particle, b2Vec2 normal) // On trouve just la nouvelle motion d'un objet.
{
	// On trouve la composente de vitesse de la particule qui est projete sur la normal ( Savoir l'amplitude de vitesse dans le meme axe que la normal avec le Dot product )
	float32 velocityPerp = b2Dot(particle.GetVelocity(), normal);
	// On trouve la vitesse perpendiculaire a l'aide de 
	b2Vec2 speedPerp = velocityPerp * normal;
	// Avec la vitesse perpendiculaire on peut trouver la vitesse paralelle vu que VT + V|| = V
	b2Vec2 speedParallel = particle.GetVelocity() - speedPerp;

	// On trouve donc la nouvelle velocit�
	b2Vec2 newVelocity = speedParallel - speedPerp;

	particle.SetVelocity(newVelocity);
}

void CollisionBoxDemo::CreateParticle()
{
	b2Vec2 boxCenter = (m_boundingBoxMax + m_boundingBoxMin) * 0.5;
	int size = m_particles.size();
	if (size < m_maxParticle)
	{
		float mass = RandomFloat(1.0, 10.0);

		m_particles.push_back(Particle(mass, b2Color(1.0 / mass, 1.0, 0.0)));
		m_particles[size].SetPosition(boxCenter);
		m_particles[size].SetVelocity(b2Vec2(RandomFloat(-30.0f, 30.0f), RandomFloat(-30.0f, 30.0f) ));
		m_particles[size].SetRadius(2.0);
	}
}

void CollisionBoxDemo::ApplyCollisionBetweenParticles(Particle& partA, Particle& partB, b2Vec2 normal)
{

	// On va vouloir faire le meme travail qu'avec les murs  mais avec les 2 particule.
	// On trouve donc la valeur de la velocite qui est allignee avec la normal de notre collision pour la particule A.


	// On va vouloir faire le meme travail qu'avec les murs  mais avec les 2 particule.
    // On trouve donc la valeur de la velocite qui est allignee avec la normal de notre collision pour la particule B. En utilisant le DOT product.
	// La normal de collision pour la particule B est l'inverse de la normal de la particule A.
	

	// Ici on cherche a trouver le vecteur de vitesse original align�e avec notre normal en multipliant l'amplitude de la vitesse allign�e avec la normal et le vecteur normal


	// On trouve la vitesse perpendiculaire a la normal en fesant la substraction vectorielle de la vitesse original et sa composant allign�e a la normal de la collision


	// Puis on calcul les nouvelle vitesse des 2 particule.

    // En on applique la vitesse au particule.

}
void CollisionBoxDemo::UpdateParticles(float deltaTime)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i].UpdateParticle(deltaTime);
		m_particles[i].RenderParticle(m_debugDraw);
	}
	CheckParticleToParticleCollision();
}
void CollisionBoxDemo::Step(Settings* settings)
{
	float deltaTime = settings->hz > 0.0f ? 1.0f / settings->hz : float32(0.0f); /// = 1 /60 ou 1/30 hz
	// Update walls 
	CheckWallCollision();
	DrawBoundingBox();
	// Update Particles
	UpdateParticles(deltaTime);

}
