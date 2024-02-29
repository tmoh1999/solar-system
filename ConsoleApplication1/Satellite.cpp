
#include "Satellite.h";
#include<iostream>

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/transform.hpp>
Satellite::Satellite() {

};

Satellite::Satellite(int t,float ModelSize, glm::vec3 pos,float dist, float diam, float planetDiam, 
	float ang1, float ang2,float spd1,float spd2)
	: Model() 
{
	type = t;
	SatelliteModelSize = ModelSize;
	PlanetPosition = glm::vec3(pos.x, pos.y, pos.z);
	distanceFromPlanet = dist;
	diametre = diam;
	planetDiametre = planetDiam;
	RotationAngleSelf = ang1;
	RotationAnglePlanet = ang2;
	SpeedRotationSelf = spd1;
	SpeedRotationPlanet = spd2;
	
	position = glm::vec3(0, 0, 0);
	CurrentSpeedRotationSelf = 0;
	CurrentSpeedRotationPlanet = 0;
};

glm::mat4 Satellite::SatelliteMouvement() {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);

	float scaleP = diametre / SatelliteModelSize;
	
	//case Sun
	if (type == 0) {
		Model = glm::scale(Model, glm::vec3(scaleP, scaleP, scaleP));
	}
	//Case Planets
	if (type == 1) {

		Model = glm::rotate(Model, CurrentSpeedRotationPlanet * glm::radians(RotationAnglePlanet),  glm::vec3(0.0f, 1.0f, 0.0f));
		
		float d = planetDiametre/2+distanceFromPlanet+diametre/2;

		glm::vec3 tmp = PlanetPosition + glm::vec3(d, 0.0f, 0.0f);
		Model = glm::translate(Model, tmp);
		
		
		Model = scale(Model, glm::vec3(scaleP, scaleP, scaleP));
		
		Model = glm::rotate(Model, CurrentSpeedRotationSelf * glm::radians(RotationAngleSelf), glm::vec3(0.0f, 1.0f, 0.0f));
		
		
		glm::vec4 m1 =  glm::vec4(0,0,0,1);
		glm::vec4 m2 = Model * m1;
		position.x = m2.x;
		position.y = m2.y;
		position.z = m2.z;

		CurrentSpeedRotationSelf += SpeedRotationSelf;
		CurrentSpeedRotationPlanet += SpeedRotationPlanet;
	}
	//case satellite 
	if (type == 2) {
		float d = planetDiametre / 2 + distanceFromPlanet + diametre / 2;


		Model = glm::translate(Model, PlanetPosition);
		Model = glm::rotate(Model, glm::radians(OrbitDegree), glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::rotate(Model, CurrentSpeedRotationPlanet * glm::radians(RotationAnglePlanet), glm::vec3(0.0f, 1.0f, 0.0f));

		
		Model = glm::translate(Model, -PlanetPosition);
		
		glm::vec3 tmp = PlanetPosition + glm::vec3(d, 0.0f, 0.0f);
		
		Model = glm::translate(Model, tmp);

		Model = scale(Model, glm::vec3(scaleP, scaleP, scaleP));


		glm::vec4 m1 = glm::vec4(0, 0, 0, 1);
		glm::vec4 m2 = Model * m1;
		position.x = m2.x;
		position.y = m2.y;
		position.z = m2.z;

		CurrentSpeedRotationPlanet += SpeedRotationPlanet;

	}
	if (CurrentSpeedRotationPlanet * RotationAnglePlanet >= 360 || CurrentSpeedRotationPlanet * RotationAnglePlanet <= -360) {
		CurrentSpeedRotationPlanet = 0;
	}

	if (CurrentSpeedRotationSelf * RotationAngleSelf >= 360 || CurrentSpeedRotationSelf * RotationAngleSelf <= -360) {
		CurrentSpeedRotationSelf = 0;
	}

	return Model;
}
void Satellite::InitOrbit(int OrbNbrVertexes, float OrbitRaduis,float orbD) {
	OrbitDegree = orbD;
	OrbitNbrVertexes = OrbNbrVertexes;
	OrbitVertexes = new STRVertex[OrbitNbrVertexes];
	STRVertex* st;
	float ml = 2 * 2 * acos(0.0);
	float x1 = 0, y1 = 0, z1 = 0;
	float r = 255, g = 255, b = 255;
	for (int i = 0; i < OrbitNbrVertexes; i++) {
		st = new STRVertex();
		x1 = OrbitRaduis * cos(i * ml / (OrbitNbrVertexes));
		z1 = -OrbitRaduis * sin(i * ml / (OrbitNbrVertexes));

		st->position = glm::vec3(x1, y1, z1);
		st->couleur = glm::vec3(r, g, b);
		st->normal = glm::vec3(0.0, 0.0, 0.0);

		OrbitVertexes[i] = *st;
	}
	glGenVertexArrays(1, &Orbit_VAO);
	glBindVertexArray(Orbit_VAO);

	glGenBuffers(1, &Orbit_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Orbit_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(OrbitVertexes[0]) * OrbitNbrVertexes, OrbitVertexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, normal));
//	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void Satellite::RenderOrbit() {
	glBindVertexArray(Orbit_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Orbit_VBO);
	glDrawArrays(GL_LINE_LOOP, 0, OrbitNbrVertexes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void Satellite:: setUniforms(GLint uniformModel , const GLfloat* Model) {
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, Model);
}
void Satellite::setPlanetPosition(glm::vec3 newpos) {
	PlanetPosition.x = newpos.x;
	PlanetPosition.y = newpos.y;
	PlanetPosition.z = newpos.z;
}
glm::vec3 Satellite::getPosition() {
	return position;
}
float Satellite::getDiametre() {
	return diametre;
}
float Satellite::getOrbitDegree()
{
	return OrbitDegree;
}
float Satellite::getDistance() {
	return planetDiametre / 2 + distanceFromPlanet + diametre / 2;
}
Satellite::~Satellite() {

};