//
//  ConstantShader.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#pragma once

#include <stdio.h>
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <iostream>
#include <assert.h>
#include "color.h"
#include "vector.h"
#include "Matrix.h"
#include "Camera.h"
#include "BaseShader.h"


class ConstantShader : public BaseShader
{
public:
    ConstantShader();
    ConstantShader(Color color);
    void color( const Color& c);
    const Color& color() const { return Col; }
    virtual void activate(const BaseCamera& Cam) const;
private:
    Color Col;
    GLuint ShaderProgram;
    GLint ColorLoc;
    GLint ModelViewProjLoc;
    
};
