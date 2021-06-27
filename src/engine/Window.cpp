#include "engine/Window.h"
#include <chrono>

#include "game/GameLogicInterface.h"
#include "yse.hpp"

#include "engine/ViewportManager.h"
#include "engine/SoundBite.h"

bool Window::framebufferResizeFlag = false;
int Window::newFramebufferWidth;
int Window::newFramebufferHeight;

int Window::preferedFramebufferWidth = 1920;

Window & Window::getWindowInstance() {
	static Window w;
	return w;
}

Window::Window() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_MAXIMIZED, true);
	title = "Ray Casting Project";
	windowHandle = glfwCreateWindow(1920, 1080, title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(windowHandle);
	glfwSwapInterval(1);

	glfwSetKeyCallback(windowHandle, keyCallback);
	glfwSetCharCallback(windowHandle, charCallback);
	glfwSetMouseButtonCallback(windowHandle, mouseButtonCallback);
	glfwSetCursorPosCallback(windowHandle, mouseMoveCallback);
	glfwSetFramebufferSizeCallback(windowHandle, windowResizeCallback);
}

void Window::mainUpdateLoop() {

	std::chrono::high_resolution_clock clock;
	std::chrono::time_point<std::chrono::steady_clock> start = clock.now();
	std::chrono::time_point<std::chrono::steady_clock> end = clock.now();

	std::chrono::time_point<std::chrono::steady_clock> startUpdateTIme = clock.now();
	float updateTime = 0.0f;
	float deltaTime = 0.0f;
	framebuffer.generateDefaultTexture(1920, 1080);
	while (!glfwWindowShouldClose(windowHandle)) {
		glfwSwapInterval(1);
		glfwSwapBuffers(windowHandle);

		startUpdateTIme = clock.now();

		glfwPollEvents();

		end = clock.now();
		deltaTime = (float)((float)(end - start).count() / 1000000.0f);
		start = clock.now();

		if (framebufferResizeFlag) {
			framebuffer.generateDefaultTexture(newFramebufferWidth, newFramebufferHeight);
			framebufferResizeFlag = false;
		}

		framebuffer.bindAsRenderTarget();
		GameLogicInterface::update(deltaTime); // renders the whole game onto the framebuffer
		framebuffer.unbindAsRenderTarget();


		static float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};
		static VertexBuffer vb = VertexBuffer(vertices, 4 * 4 * sizeof(float));
		
		static unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3
		};
		static IndexBuffer ib = IndexBuffer(indices, 6 * sizeof(unsigned int));
		
		static VertexArray va = VertexArray("ff ff", vb, ib);
		
		if (!postProcessingShader) {
			framebuffer.bind(1);
			defaultPostProcessingShader.setUniform1i("u_texture", 1);

			ViewportManager::bindViewportPixels(0, 0, getWidth(), getHeight());
			defaultPostProcessingShader.bind();
			va.bind();
			glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr); // draws the framebuffer stretched over the whole screen
			va.unbind();
			defaultPostProcessingShader.unbind();
			ViewportManager::unbindViewport();
		}
		else {
			framebuffer.bind(1);
			postProcessingShader->setUniform1i("u_texture", 1);

			ViewportManager::bindViewportPixels(0, 0, getWidth(), getHeight());
			postProcessingShader->bind();
			va.bind();
			glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr); // draws the framebuffer stretched over the whole screen
			va.unbind();
			postProcessingShader->unbind();
			ViewportManager::unbindViewport();
		}

		updateTime = (float)(((double)(clock.now() - startUpdateTIme).count()) / 1000000.0f);

		ViewportManager::update();
		YSE::System().update();
		SoundBite::update();
		glfwMakeContextCurrent(windowHandle);

		static uint64_t frame = 0;
		frame++;
		if (frame % 10 == 0) {
			calculateFPS();

			char t[256];
			sprintf_s(t, "%s - FPS: %.1f - delta time(%.2fms) - update time(%fms)", title.c_str(), (float)getFrameRate(), deltaTime, updateTime);
			glfwSetWindowTitle(windowHandle, t);
		}
	}

}

void Window::setSize(int width, int height) {
	glfwSetWindowSize(windowHandle, width, height);
}

void Window::setTitle(const std::string & title) {
	this->title = title;
}

int Window::getWidth() {
	int width;
	glfwGetWindowSize(windowHandle, &width, nullptr);
	return width;
}

int Window::getHeight() {
	int height;
	glfwGetWindowSize(windowHandle, nullptr, &height);
	return height;
}

std::string Window::getTitle() {
	return title;
}

int Window::getFrameBufferWidth()
{
	return framebuffer.getWidth();
}

int Window::getFrameBufferHeight()
{
	return framebuffer.getHeight();
}

float Window::getAspectRatio() {
	int width, height;
	glfwGetWindowSize(windowHandle, &width, &height);
	return (float)width / height;
}

double Window::getFrameRate(){
	return fps;
}

bool Window::keyIsDown(int glfwKey) {
	int state = glfwGetKey(windowHandle, glfwKey);
	return state == GLFW_PRESS;
}

float Window::getMouseX() {
	return mouseX;
}

float Window::getMouseY() {
	return mouseY;
}

void Window::close() {
	glfwSetWindowShouldClose(windowHandle, true);
}

GLFWwindow * Window::getHandle() {
	return windowHandle;
}

Texture& Window::getFramebufferTexture()
{
	return framebuffer;
}

void Window::setPostProcessingShaderDefault()
{
	postProcessingShader = &defaultPostProcessingShader;
}

void Window::setPostProcessingShader(Shader& shader)
{
	postProcessingShader = &shader;
}

void Window::calculateFPS() {
	static std::chrono::high_resolution_clock clock;
	static long long timeA;
	static long long timeB;
	timeA = clock.now().time_since_epoch().count();

	long long deltaTime = timeA - timeB;

	timeB = clock.now().time_since_epoch().count();

	fps =  1.0f / ((double)(deltaTime / 10) / 1000000000);
}

void Window::setResolution(int width, int height) 
{
	preferedFramebufferWidth = width;

	framebufferResizeFlag = true;
	newFramebufferWidth = preferedFramebufferWidth;
	newFramebufferHeight = (preferedFramebufferWidth * getHeight()) / getWidth();
}

void Window::keyCallback(GLFWwindow * wind, int key, int scancode, int action, int mods) {
	GameLogicInterface::keyCallback(key, scancode, action, mods);
}

void Window::charCallback(GLFWwindow * wind, unsigned int codepoint) {
	GameLogicInterface::characterCallback(codepoint);
}

void Window::mouseButtonCallback(GLFWwindow * wind, int button, int action, int mods) {
	GameLogicInterface::mouseButtonCallback(button, action, mods);
}

void Window::mouseMoveCallback(GLFWwindow * wind, double xPos, double yPos) {
	int width, height;
	glfwGetWindowSize(window.windowHandle, &width, &height);
	window.mouseX = ((float)(xPos - width / 2) / (width / 2)) * window.getAspectRatio();
	window.mouseY = (float)(-yPos + height / 2) / (height / 2);
	GameLogicInterface::mouseMoveCallback(xPos, yPos);
}

void Window::windowResizeCallback(GLFWwindow* wind, int width, int height)
{
	framebufferResizeFlag = true;
	//newFramebufferWidth = preferedFramebufferWidth;
	//newFramebufferHeight = (preferedFramebufferWidth * height) / width;

	newFramebufferWidth = width;
	newFramebufferHeight = height;
}
