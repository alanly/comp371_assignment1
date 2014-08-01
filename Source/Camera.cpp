//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "Camera.h"
#include <GLM/gtx/transform.hpp>

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	// @TODO 1 - Calculate View Projection Matrix
	//glm::mat4 viewProjection(1.0f); // identity, you need to change this
	
	// Calculate our ViewProjection.
	glm::mat4 viewProjection = GetProjectionMatrix()*GetViewMatrix();

	return viewProjection;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(
		glm::vec3(4, 3, 3),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}
