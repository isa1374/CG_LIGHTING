#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else
	#include "freeglut.h"
#endif

#include "vecs.h"

#ifndef __C_PARTICLE_H__
#define __C_PARTICLE_H__

#define DRAGCOEFF 0.9

class cParticle
{

public:
	// constructor, destructor
	cParticle();
	cParticle(float m, float *pos0);
	~cParticle();

	// draw the particle
	void draw();
	void stepSimulation(float dt);
	// add a force to the particle
	void addForce(float *force);
	void checkFloorCollisions(); 

	bool inCollision(cParticle* other);

	bool alive;
	float diffuse[3];
	float oDiffuse[3];
	float specular[3];
	float shininess; 
	float radius; 
	float restCoefficient; 

	// remove all forces on this particle
	void clearForces();

	// find the new speed and position
	void integrateVerlet(float dt);

	// physical properties
	float mass;			// mass
	float pos[3];		// position
	float oldPos[3];    // previous position, required by Verlet's integration
	float forces[3];	// force accumulator 
	float dragForce[3]; // other forces, like the wind
};

#endif