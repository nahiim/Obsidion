
#pragma once

#include <iostream>

#include <vulkan/vulkan.hpp>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>



class Window
{
public:

	//Screen dimension constants
	unsigned int SCREEN_WIDTH = 640;
	unsigned int SCREEN_HEIGHT = 480;

	float dt, t1, t2 = 0;
	float start_time, u_time;

	char *title;

	unsigned int sdl_extension_count;
	std::vector<const char*> sdl_extensions;


	Window()
	{
		init();
	}

	~Window()
	{
	    //Disable text input
	    SDL_StopTextInput();

	    //Destroy window
	    SDL_DestroyWindow( window );
	    window = NULL;

	    //Quit SDL subsystems
	    SDL_Quit();
	}



	void loop(bool &running)
	{
        t1 = SDL_GetTicks();
        dt = t1 - t2;
        t2 = t1;

        u_time = SDL_GetTicks() - start_time;  

        //Handle events on queue
        // while (SDL_PollEvent(&event))
        // {
        //     //close the window when user alt-f4s or clicks the X button
        //     if (event.type == SDL_QUIT)
        //     {
        //         running = false;
        //     }
        // }
	}

	void get_extensions()
	{
		// Get the required extension count
		if (!SDL_Vulkan_GetInstanceExtensions(window, &sdl_extension_count, nullptr))
		{
			std::cout << "Failed to get SDL extensions\n";
		}

		sdl_extensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME }; // Sample additional extension 
		size_t additional_extension_count = sdl_extensions.size();
		sdl_extensions.resize(additional_extension_count + sdl_extension_count);

		if (!SDL_Vulkan_GetInstanceExtensions(window, &sdl_extension_count, sdl_extensions.data() + additional_extension_count))
		{
			std::cout << "Failed to get SDL extensions\n";
		}
	}



	void create_surface(vk::SurfaceKHR &surface, vk::Instance &instance)
	{
		VkSurfaceKHR c_style_surface;

		if (SDL_Vulkan_CreateSurface(window, instance, &c_style_surface) != SDL_TRUE)
		{
			std::cout << "Failed to abstract SDL surface for Vulkan\n";
		}

		//copy constructor converts to hpp convention
		surface = c_style_surface;
	}

	void get_framebuffer_size(int &width, int &height)
	{
		SDL_Vulkan_GetDrawableSize(window, &width, &height);
	}



private:

	SDL_Window *window;
	SDL_Event event;


	void init()
	{
	    //initialize SDL
	    SDL_Init(SDL_INIT_VIDEO);

	    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

	    window = SDL_CreateWindow(
	        "Vulkan Engine",
	        SDL_WINDOWPOS_UNDEFINED,
	        SDL_WINDOWPOS_UNDEFINED,
	        SCREEN_WIDTH, SCREEN_HEIGHT,
	        window_flags
	    );
	}
};