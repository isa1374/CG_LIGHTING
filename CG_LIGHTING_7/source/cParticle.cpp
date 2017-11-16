#include "cParticle.h"

cParticle::cParticle()
{
	restCoefficient = 0.8f;
	radius = 10.0f + 5.0f * ((float)rand() / (float)RAND_MAX);
	mass = radius;
	pos[0] = 1000.0f*((float)rand() / (float)RAND_MAX);
	pos[1] = 200.0f + 100.0f *((float)rand() / (float)RAND_MAX);
	pos[2] = 1000.0f*((float)rand() / (float)RAND_MAX);

	oldPos[0] = pos[0];
	oldPos[1] = pos[1];
	oldPos[2] = pos[2];

	forces[0] = -10 + 20.0f * ((float)rand()/(float)RAND_MAX);
	forces[1] = 0.0;
	forces[2] = -10 + 20.0f * ((float)rand() / (float)RAND_MAX);

	diffuse[0] = 0.8f*((float)rand() / (float)RAND_MAX);
	diffuse[1] = ((float)rand() / (float)RAND_MAX);
	diffuse[2] = ((float)rand() / (float)RAND_MAX);

	oDiffuse[0] = diffuse[0];
	oDiffuse[1] = diffuse[1];
	oDiffuse[2] = diffuse[2]; 

	specular[0] = 0.8;
	specular[1] = 0.8;
	specular[2] = 0.8; 

	shininess = 64; 

	dragForce[0] = 0.0;
	dragForce[1] = 0.0;
	dragForce[2] = 0.0;

	alive = true;

}

cParticle::cParticle(float m, float *pos0)
{
	mass = m;
	pos[0] = pos0[0];
	pos[1] = pos0[1];
	pos[2] = pos0[2];
	oldPos[0] = pos[0];
	oldPos[1] = pos[1];
	oldPos[2] = pos[2];

	dragForce[0] = 0.0;
	dragForce[1] = 0.0;
	dragForce[2] = 0.0;

	alive = false;
}

cParticle::~cParticle()
{
}

void cParticle::draw()
{
	glPushMatrix(); {
		glEnable(GL_COLOR_MATERIAL); {
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular); 
			glMaterialf(GL_FRONT, GL_SHININESS, shininess);
			glEnable(GL_COLOR_MATERIAL); 
			glColor3fv(diffuse);
			glTranslatef(pos[0], pos[1], pos[2]);
			glutSolidSphere(radius, 10, 10);
		}
		glDisable(GL_COLOR_MATERIAL); 
	}
	glPopMatrix();
}

void cParticle::stepSimulation(float dt)
{
	integrateVerlet(dt);
}

void cParticle::clearForces()
{
	forces[0] = 0;
	forces[1] = 0;
	forces[2] = 0;
}

void cParticle::addForce(float *force)
{
	forces[0]+=force[0];
	forces[1]+=force[1];
	forces[2]+=force[2];

	dragForce[0]=forces[0];
	dragForce[1]=forces[1];
	dragForce[2]=forces[2];

	unitVector(dragForce);
}

void cParticle::integrateVerlet(float dt)
{
	float accel[3], posAux[3];

	// Apply 2nd Newton law: f = ma ->  a = f / m
	accel[0] = forces[0]/mass;
	accel[1] = forces[1]/mass;
	accel[2] = forces[2]/mass;

	posAux[0] = pos[0];
	posAux[1] = pos[1];
	posAux[2] = pos[2];

	pos[0] += pos[0] - oldPos[0] + accel[0]*dt*dt - dragForce[0]; 
	pos[1] += pos[1] - oldPos[1] + accel[1]*dt*dt - dragForce[1];
	pos[2] += pos[2] - oldPos[2] + accel[2]*dt*dt - dragForce[2];

	oldPos[0] = pos[0];
	oldPos[1] = pos[1];
	oldPos[2] = pos[2];
}


bool cParticle::inCollision(cParticle* other) {
	// red = (0.55, 0.09, 0.09)
	float r1 = radius;
	float r2 = other->radius;
	float x1 = pos[0];
	float x2 = other->pos[0];
	float y1 = pos[1];
	float y2 = other->pos[1];
	float z1 = pos[2];
	float z2 = other->pos[2]; 

	float dx2 = (x2 - x1)*(x2 - x1);
	float dy2 = (y2 - y1)*(y2 - y1);
	float dz2 = (z2 - z1)*(z2 - z1); 

	float dist = dx2 + dy2 + dz2;
	float sumR2 = (r1 + r2) * (r1 + r2); 


	if (dist < sumR2) {
		return true; 
 	}
	return false; 
}

void cParticle::checkFloorCollisions() {
	if (pos[1] < radius) {
		
		pos[1] = radius; 
		forces[1] = -restCoefficient * forces[1]; //at leat 0.8f

		float dirX = pos[0] - oldPos[0];
		float dirZ = pos[2] - oldPos[2]; 

		float dX2 = dirX * dirX; 
		float dZ2 = dirZ * dirZ; 
		float mag = sqrt(dX2 + dZ2); 

		dirX /= mag; 
		dirZ /= mag; 

		dragForce[0] = dirX; 
		dragForce[1] = forces[1]; 
		dragForce[2] = dirZ; 
		unitVector(dragForce);
	}
}