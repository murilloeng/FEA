// std
#include <thread>
#include <stdexcept>

// FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Draw.hpp"
#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/Solver.hpp"

// Canvas
#include "Canvas/inc/API/Loader.hpp"

namespace fea
{
	namespace draw
	{
		// destructor
		Engine::Engine(const Model *model) : m_draw{nullptr}, m_playing{false}, m_show_fps{false}, m_framerate{60}, m_model{model}
		{
			setup_glfw();
			setup_scene();
			setup_callbacks();
		}

		// destructort
		Engine::~Engine(void)
		{
			delete m_scene;
			glfwTerminate();
		}

		// start
		void Engine::start(void)
		{
			glfwSetTime(0);
			float t1 = 0, t2;
			while (!glfwWindowShouldClose(m_window))
			{
				// time
				t2 = (float)glfwGetTime();
				// play
				if (m_playing)
				{
					update_step(m_window, true);
					m_scene->update();
					const uint32_t t = uint32_t(1000 / m_framerate);
					std::this_thread::sleep_for(std::chrono::milliseconds(t));
				}
				// animations
				glfwPollEvents();
				m_scene->time(t2);
				m_scene->update_animations();
				// draw
				m_scene->draw();
				glfwSwapBuffers(m_window);
				// framerate
				print_fps(t1, t2);
			}
		}

		// data
		bool Engine::show_fps(void) const
		{
			return m_show_fps;
		}
		bool Engine::show_fps(bool show_fps)
		{
			return m_show_fps = show_fps;
		}

		double Engine::framerate(void) const
		{
			return m_framerate;
		}
		double Engine::framerate(double framerate)
		{
			return m_framerate = framerate;
		}

		// setup
		void Engine::setup_glfw(void)
		{
			// library
			if (glfwInit() != GLFW_TRUE)
			{
				throw std::runtime_error("GLFW initialization failed!");
			}
			// window
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			m_window = glfwCreateWindow(900, 900, "Canvas", nullptr, nullptr);
			if (!m_window)
			{
				glfwTerminate();
				throw std::runtime_error("GLFW window initialization failed!");
			}
			// context
			glfwMakeContextCurrent(m_window);
			// v-sync
			glfwSwapInterval(0);
			// functions
			canvas::load_functions();
		}
		void Engine::setup_scene(void)
		{
			// scene
			m_scene = new canvas::Scene;
			canvas::shaders::Shader::add_path("../Canvas/shd/");
			// draw
			m_draw = new Draw{m_model};
			m_draw->compute_bounding_box();
			// objects
			m_scene->add_object(m_draw);
			// camera
			m_scene->setup();
			m_scene->update();
			m_scene->camera().fixed_bounding_box(true);
			m_scene->camera().bounding_box(m_draw->m_bounding_box);
			// screen
			int32_t width, height;
			glfwGetWindowSize(m_window, &width, &height);
			m_scene->camera().callback_reshape(width, height);
		}
		void Engine::setup_callbacks(void)
		{
			glfwSetWindowUserPointer(m_window, this);
			glfwSetKeyCallback(m_window, callback_keyboard);
			glfwSetScrollCallback(m_window, callback_wheel);
			glfwSetWindowSizeCallback(m_window, callback_resize);
			glfwSetCursorPosCallback(m_window, callback_position);
			glfwSetMouseButtonCallback(m_window, callback_button);
		}

		// print
		void Engine::print_fps(float &t1, float t2)
		{
			t1 = t2;
			if (m_show_fps)
				printf("FPS: %d\n", uint32_t(1 / (t2 - t1)));
		}

		// update
		void Engine::update_playing(GLFWwindow *window)
		{
			// data
			Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			// playing
			engine->m_playing = !engine->m_playing;
		}
		void Engine::update_step(GLFWwindow *window, bool increase)
		{
			// data
			const Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			// step
			const uint32_t is = engine->m_draw->m_step;
			const uint32_t ns = engine->m_model->analysis()->solver()->draw_steps();
			const uint32_t js = increase ? (is + 1) % ns : is != 0 ? is - 1 : ns - 1;
			// draw
			engine->m_draw->step(js);
			if (!engine->m_show_fps)
				printf("step: %d\n", js);
			// update
			engine->m_scene->update();
		}
		void Engine::update_framerate(GLFWwindow *window, bool increase)
		{
			// data
			const double factor = 1.10;
			Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			// framerate
			engine->m_framerate *= increase ? factor : 1 / factor;
		}

		// canvas
		uint32_t Engine::canvas_modifiers(int32_t glfw_modifiers)
		{
			// data
			int32_t glfw_list[] = {
				GLFW_MOD_ALT, GLFW_MOD_SHIFT, GLFW_MOD_CONTROL};
			canvas::modifier canvas_list[] = {
				canvas::modifier::alt, canvas::modifier::shift, canvas::modifier::control};
			// modifiers
			uint32_t canvas_modifiers = 0;
			for (uint32_t i = 0; i < 3; i++)
			{
				if (glfw_modifiers & glfw_list[i])
				{
					canvas_modifiers |= uint32_t(canvas_list[i]);
				}
			}
			// return
			return canvas_modifiers;
		}
		canvas::button Engine::canvas_button(int32_t glfw_button)
		{
			// data
			int32_t glfw_list[] = {
				GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE};
			canvas::button canvas_list[] = {
				canvas::button::left, canvas::button::right, canvas::button::middle};
			// modifiers
			for (uint32_t i = 0; i < 3; i++)
			{
				if (glfw_button == glfw_list[i])
				{
					return canvas_list[i];
				}
			}
			// return
			return canvas::button::none;
		}

		// callbacks
		void Engine::callback_wheel(GLFWwindow *window, double dx1, double dx2)
		{
			// data
			Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			// camera
			engine->m_scene->camera().callback_wheel(dx2 < 0);
			// update
			glfwSwapBuffers(window);
		}
		void Engine::callback_button(GLFWwindow *window, int32_t button, int32_t action, int32_t modifiers)
		{
			// data
			double x1, x2;
			glfwGetCursorPos(window, &x1, &x2);
			Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			// callback
			engine->m_scene->camera().callback_mouse(canvas_button(button), action == GLFW_PRESS, int32_t(x1), int32_t(x2), canvas_modifiers(modifiers));
			// update
			glfwSwapBuffers(window);
		}
		void Engine::callback_resize(GLFWwindow *window, int32_t width, int32_t height)
		{
			// data
			Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			// camera
			engine->m_scene->camera().callback_reshape(width, height);
		}
		void Engine::callback_position(GLFWwindow *window, double x1, double x2)
		{
			Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			engine->m_scene->camera().callback_motion(int32_t(x1), int32_t(x2));
			glfwSwapBuffers(window);
		}
		void Engine::callback_keyboard(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
		{
			// data
			if (action == GLFW_RELEASE)
				return;
			const bool shift = mods & GLFW_MOD_SHIFT;
			const bool control = mods & GLFW_MOD_CONTROL;
			const char *key_name = glfwGetKeyName(key, 0);
			Engine *engine = (Engine *)glfwGetWindowUserPointer(window);
			// play
			if (shift && key == GLFW_KEY_P)
				update_playing(window);
			// step
			else if (shift && key == GLFW_KEY_LEFT)
				update_step(window, false);
			else if (shift && key == GLFW_KEY_RIGHT)
				update_step(window, true);
			// framerate
			else if (shift && key == GLFW_KEY_S)
				update_framerate(window, true);
			else if (control && key == GLFW_KEY_S)
				update_framerate(window, false);
			// glfw
			else if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
			// camera
			else if (key_name)
				engine->m_scene->camera().callback_key(*key_name);
			else if (key == GLFW_KEY_MINUS)
				engine->m_scene->camera().callback_wheel(true);
			else if (shift && key == GLFW_KEY_EQUAL)
				engine->m_scene->camera().callback_wheel(false);
		}
	}
}