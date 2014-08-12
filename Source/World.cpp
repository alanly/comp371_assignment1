//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "FirstPersonCamera.h"

#include "CubeModel.h"
#include "VehicleModel.h"
#include "SphereModel.h"

#include "PointLight.h"
#include "DirectionalLight.h"

#include <GLFW/glfw3.h>
#include "EventManager.h"

using namespace std;
using namespace glm;

World::World()
{
	// Setup Camera
	mCamera.push_back( new StaticCamera( vec3(3.0f, 4.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) ) );
	mCamera.push_back( new FirstPersonCamera( vec3(0.5f, 0.5f, 5.0f) ) );
	mCurrentCamera = 0;

	// Setup Light
	mCurrentLight = 0;

	// The geometry should be loaded from a scene file
}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}
	mModel.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();

	// Lights
	for (vector<Light*>::iterator it = mLight.begin(); it < mLight.end(); ++it)
	{
		delete *it;
	}
	mLight.clear();
}
	
void World::Update(float dt)
{
	// User Inputs
	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
	{
		mCurrentCamera = 0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 1)
		{
			mCurrentCamera = 1;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 2)
		{
			mCurrentCamera = 2;
		}
	}

	// Spacebar to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_SOLID_COLOR);
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_BLUE);
	}

	// L to change Light
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_L) == GLFW_PRESS)
	{
		mCurrentLight++;

		if (mCurrentLight >= mLight.size()) {
			mCurrentLight = 0;
		}

		printf("Using light: %s\n", mLight[mCurrentLight]->GetLightName().c_str());
	}

	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set Shader... In a more sofisticated application, each model could use a different shader
	// In our case, all the models use a common shader
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectonTransform");

	// Handles for transformation matrices.
	GLuint ViewMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectonTransform");

	// Get the number of lights
	int mLightSize = mLight.size();

	// Declare the uniform pointers, holding our light values
	GLfloat* lightPositions    = new GLfloat[mLightSize * 4];
	GLfloat* lightColors       = new GLfloat[mLightSize * 3];
	GLfloat* lightCoefficients = new GLfloat[mLightSize * 3];

	// Populate with the light values
	for (int i = 0; i < mLightSize; i++)
	{
		// Populate lightPositions (vec4)
		for (int p = 0; p < 4; p++) {
			lightPositions[(i * 4) + p] = mLight[i]->GetLightPosition()[p];
		}

		// Populate lightColors & lightCoefficients (vec3)
		for (int c = 0; c < 3; c++) {
			int index = (i * 3) + c;
			lightColors[index] = mLight[i]->GetLightColor()[c];
			lightCoefficients[index] = mLight[i]->GetLightCoefficients()[c];
		}
	}

	// Draw models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		// Send the view-projection matrices to the shader
		mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &mCamera[mCurrentCamera]->GetViewMatrix()[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &mCamera[mCurrentCamera]->GetProjectionMatrix()[0][0]);

		// Send the light properties to the shader
		glUniform1i(Renderer::GetLightSizeUniformID(), mLightSize);
		glUniform4fv(Renderer::GetLightPositionsUniformID(), mLightSize, lightPositions);
		glUniform3fv(Renderer::GetLightColorsUniformID(), mLightSize, lightColors);
		glUniform3fv(Renderer::GetLightAttenuationsUniformID(), mLightSize, lightCoefficients);
				
		// Send the material coefficients for this model.
		glUniform4f(
			Renderer::GetShaderMaterialID(), 
			(*it)->GetMaterialCoefficients().x,
			(*it)->GetMaterialCoefficients().y,
			(*it)->GetMaterialCoefficients().z,
			(*it)->GetMaterialCoefficients().w
		);

		// Draw model
		(*it)->Draw();
	}

	Renderer::EndFrame();

	delete lightPositions;
	delete lightColors;
	delete lightCoefficients;
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{
			if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);
			}
			else if( result == "vehicle" )
			{
				// Box attributes
				VehicleModel* vehicle = new VehicleModel();
				vehicle->Load(iss);
				mModel.push_back(vehicle);
			}
			else if( result == "sphere" )
			{
				// Box attributes
				SphereModel* sphere = new SphereModel();
				sphere->Load(iss);
				mModel.push_back(sphere);
			}
			else if ( result == "pointlight" )
			{
				PointLight* light = new PointLight();
				light->Load(iss);
				mLight.push_back(light);
			}
			else if ( result == "directionallight" )
			{
				DirectionalLight* light = new DirectionalLight();
				light->Load(iss);
				mLight.push_back(light);
			}
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}
			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();

	printf("%i Lights Loaded.\n", mLight.size());
}
