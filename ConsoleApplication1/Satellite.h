#pragma once
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct STRVertex
{
	glm::vec3 position;
	glm::vec3 couleur;
	glm::vec3 normal;
};

class Satellite :
	public Model 
{
public:
	Satellite();
	Satellite(int typefloat, float SatelliteModelSize ,glm::vec3 PlanetPosition,float distanceFromPlanet,float diametre,
		float planetDiametre,float RotationAngleSelf,float RotationAnglePlanet,
		float SpeedRotationSelf, float SpeedRotationPlanet);

	glm::mat4 SatelliteMouvement();
	float getDistance();
	float getDiametre();
	void InitOrbit(int OrbNbrVertexes, float OrbitRaduis,float OrbitDegree);
	void RenderOrbit();
	float getOrbitDegree();
	glm::vec3 getPosition();
	void setPlanetPosition(glm::vec3 newpos);
	void setUniforms(GLint uniformModel,const GLfloat* Model);
	~Satellite();

private:
	int type;
	float SatelliteModelSize;
	glm::vec3 position;
	glm::vec3 PlanetPosition;
	float distanceFromPlanet;
	float planetDiametre;
	float diametre;

	GLuint Orbit_VAO;
	GLuint Orbit_VBO;
	int OrbitNbrVertexes;
	STRVertex *OrbitVertexes;
	float OrbitDegree;

	float RotationAngleSelf;
	float RotationAnglePlanet;
	float SpeedRotationSelf;
	float SpeedRotationPlanet;
	float CurrentSpeedRotationSelf;
	float CurrentSpeedRotationPlanet;
};