//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#pragma once

struct GLFWwindow;

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>


enum ShaderType
{
	SHADER_SOLID_COLOR,
	SHADER_BLUE,
	NUM_SHADERS
};


class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

	static GLuint LoadShaders(const char * vertex_shader_path, const char * fragment_shader_path);
	static bool   LoadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals );

	static unsigned int GetShaderProgramID() { return sShaderProgramID[sCurrentShader]; }
	static void SetShader(ShaderType type);

	static unsigned int GetShaderLightPositionID() { return sShaderLightPositionID; }
	static unsigned int GetShaderLightColorID() { return sShaderLightColorID; }
	static unsigned int GetShaderLightAttenuationID() { return sShaderLightAttenuationID; }
	static unsigned int GetShaderMaterialID() { return sShaderMaterialID; }

private:
	static GLFWwindow* spWindow;

	static std::vector<unsigned int> sShaderProgramID;
	static unsigned int sCurrentShader;

	static unsigned int sShaderLightPositionID;
	static unsigned int sShaderLightColorID;
	static unsigned int sShaderLightAttenuationID;
	static unsigned int sShaderMaterialID;

};

