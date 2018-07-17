#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include "GPUProgramParameter.h"

namespace Nova {
	GPUProgramParameterMat4::GPUProgramParameterMat4(const GLuint location, const float * parameter)
		:mData(parameter), mLocation(location)
	{
	}
	void GPUProgramParameterMat4::update() //warning: program has to be already binded!
	{
		glUniformMatrix4fv(mLocation , 1, GL_FALSE, mData);
	}

	GPUProgramParameterVec3::GPUProgramParameterVec3(const GLuint location, const float * parameter)
		:mData(parameter), mLocation(location)
	{
	}	
	void GPUProgramParameterVec3::update() //warning: program has to be already binded!
	{
		glUniform3fv(mLocation, 1, mData);
	}

	GPUProgramParameterVec4::GPUProgramParameterVec4(const GLuint location, const float * parameter)
		:mData(parameter), mLocation(location)
	{
	}
	void GPUProgramParameterVec4::update() //warning: program has to be already binded!
	{
		glUniform4fv(mLocation, 1, mData);
	}
	GPUProgramParameterFloat::GPUProgramParameterFloat(const GLuint location, const float parameter)
		:mData(parameter), mLocation(location)
	{
	}
	void GPUProgramParameterFloat::update()
	{
		glUniform1f(mLocation, mData);
	}
	GPUProgramParameterInt::GPUProgramParameterInt(const GLuint location, const int parameter)
		:mData(parameter), mLocation(location)
	{
	}
	void GPUProgramParameterInt::update()
	{
		glUniform1i(mLocation, mData);
	}
}

