
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

// #include <GLFW/glfw3.h>
// #include "RendererBackendForward.h"
// #include "graphics/opengl/GraphicsSystem.h"
// #include "math/Vector.h"
// #include "math/Matrix.h"
// #include "resource_manager/ResourceManager.h"
// #include "Application.h"
// #include "graphics/opengl/GPUProgram.h"

// namespace Nova {
// 	//RendererBackend::RendererBackend()
// 	//	:mFrameBuffer(FrameBuffer::makeGBuffer(Window::getInstance().getWidth(),
// Window::getInstance().getHeight())),
// 	//	mFullScreenQuadPacket(std::make_shared<Mesh>(Mesh::makeQuad(2.0f, 2.0f)),
// 	//
// std::make_shared<Material>(ResourceManager::getInstance().get<GPUProgram>("framebuffer"),
// 	//
// std::vector<std::pair<std::shared_ptr<ITexture>,std::string>>(std::make_pair <
// mFrameBuffer.getColorTexture(0), std::string{ "diffuse" }  )))
// 	//{
// 	//	glActiveTexture(GL_TEXTURE0); //activate it in case the driver doesn't activate it by
// default
// 	//	//LOG_DEBUG("fb complete:" << mFrameBuffer.isComplete());
// 	//	/*int res;
// 	//	glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
// 	//	LOG_DEBUG("Max Framebuffer Samples:" << res);

// 	//	glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
// 	//	LOG_DEBUG("Max Framebuffer Layers:" << res);*/
// 	//}
// 	void RendererBackend::render()
// 	{
// 		auto &renderPackets = GraphicsSystem::getInstance().getRenderPackets();
// 		auto &lights = GraphicsSystem::getInstance().getLights();
// 		Mat4 ortho = Mat4::makeOrthographicMatrix(1280.0f, 720.0f, 0.01f, 100.0f);
// 		Mat4 view = Mat4::makeLookAtMatrix(Vec3{ 5.0f,5.0f,0.0f }, Vec3{ 0.0f,0.0f,0.0f },
// Vec3{ 0.0f,1.0f,0.0f }); 		Mat4 lightSpaceMatrix = ortho * view; 		auto &rm =
// ResourceManager::getInstance(); 		auto shader = rm.get<GPUProgram>("shadowmap"); 		auto& engine =
// Application::getInstance(); 		lightSpaceMatrix = lightSpaceMatrix *
// engine.getWorld().getEntity("sun_light").getFinalTransform();
// 		////lights[1].

// 		shader->bind();
// 		glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "lightSpaceMatrix"),
// 			1, GL_FALSE, lightSpaceMatrix.getDataPtr());
// 		//glViewport(0, 0, DEFAULT_SHADOW_MAP_WIDTH, DEFAULT_SHADOW_MAP_HEIGHT);

// 		mFrameBuffer.bind();
// 		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 		for (const auto & packet : renderPackets)
// 		{
// 			packet.bind();
// 			packet.updateAllUniforms();
// 			packet.draw();
// 			packet.unBind();
// 		}
// 		//Draws the skybox
// 		/*mCurrentSkyBox->bind();
// 		mCurrentSkyBox->updateAllUniforms();
// 		mCurrentSkyBox->draw();
// 		mCurrentSkyBox->unBind();*/
// 		mFrameBuffer.unBind();

// 		renderPackets.clear();
// 		lights.clear();

// 		glDisable(GL_DEPTH_TEST);
// 		//glClear(GL_COLOR_BUFFER_BIT);

// 		mFullScreenQuadPacket.bind();
// 		//mFullScreenQuadPacket.updateAllUniforms();
// 		mFullScreenQuadPacket.draw();
// 		mFullScreenQuadPacket.unBind();
// 		glEnable(GL_DEPTH_TEST);

// 		//CHECK_OPEGL_ERROR();
// 	}
// }