#pragma once
#include <glad/glad.h>
#include "graphics/IGPUProgram.h"
#include "graphics/IGPUProgramParameter.h"

namespace Nova {
	class GPUProgram : public IGPUProgram
	{
	public:					
		//GPUProgram(const std::string &vertexShader, const std::string &pixelShader);
		GPUProgram(const std::string &fileAndPath);
		~GPUProgram();

		void bind() const override;
		void unBind() const override;
		GLuint getProgramID() const { return mProgramID; }
		
	private:
		const GLuint mProgramID;		

		void construct(const std::string &vertexShader, const std::string &pixelShader, const std::string &filename);
	};	
}
