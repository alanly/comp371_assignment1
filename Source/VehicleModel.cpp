//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "VehicleModel.h"
#include "CubeModel.h"
#include <GLFW/glfw3.h>


using namespace std;
using namespace glm;

VehicleModel::VehicleModel() : mPropellerRotAngleInDegrees(0.0f)
{
	// @TODO 5 - Layout your vehicle in a hierarchy
	
	// Our parent model, the "fuselage" or body section of the plane.
	CubeModel* fuselage = new CubeModel(glm::vec3(1.0f, 1.0f, 3.0f));
	cubes.push_back(fuselage);

	// All direct children of the fuselage (i.e. components that are immediately attached to the fuselage)
	CubeModel* wing = new CubeModel(fuselage, glm::vec3(5.0f, 0.125f, 1.0f));
	wing->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cubes.push_back(wing);

	CubeModel* nose = new CubeModel(fuselage, glm::vec3(0.25f, 0.25f, 0.25f));
	nose->SetPosition(glm::vec3(0.0f, 0.0f, 1.625f));
	cubes.push_back(nose);
	
	CubeModel* tailfin = new CubeModel(fuselage, glm::vec3(0.125f, 1.0f, 1.0f));
	tailfin->SetPosition(glm::vec3(0.0f, 0.5f, -1.5f));
	tailfin->SetRotation(glm::vec3(-1.0f, 0.0f, 0.0f), 25.0f);
	cubes.push_back(tailfin);
	
	// The properllers which are child components of the wing section.
	CubeModel* leftPropeller = new CubeModel(wing, glm::vec3(0.0625f, 1.0f, 0.0625f));
	leftPropeller->SetPosition(glm::vec3(-1.5f, 0.0f, 0.53125f));
	cubes.push_back(leftPropeller);

	CubeModel* rightPropeller = new CubeModel(wing, glm::vec3(0.0625f, 1.0f, 0.0625f));
	rightPropeller->SetPosition(glm::vec3(1.5f, 0.0f, 0.53125f));
	cubes.push_back(rightPropeller);
}

VehicleModel::~VehicleModel()
{
}

void VehicleModel::Update(float dt)
{	
	// @TODO 6 (Optional) - Here is where you can add some code if you want your vehicle to animate
	mPropellerRotAngleInDegrees += 1440 * dt;
}

void VehicleModel::Draw()
{
	// @TODO 5 - Draw Each part of your vehicle here
	
	// Adjust our parent model
	CubeModel* parent = cubes.at(0);
	parent->SetPosition(mPosition);
	parent->SetRotation(mRotationAxis, mRotationAngleInDegrees);
	parent->SetScaling(mScaling);

	for (uint i = 0; i < cubes.size(); ++i)
	{
		cubes.at(i)->Draw();
	}

	for (uint i = (cubes.size() - 2); i < cubes.size(); ++i)
	{
		CubeModel* propeller = cubes.at(i);
		propeller->SetRotation(glm::vec3(0.0f, 0.0f, 1.0f), mPropellerRotAngleInDegrees);
	}
}

bool VehicleModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}
