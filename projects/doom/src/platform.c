#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "platform.h"

#define GFX_WIDTH             (320*2)
#define GFX_HEIGHT            (200*2)

#define WINDOW_TITLE             "DOOM"

static uint8_t *_palette;
static uint32_t _pixels[GFX_HEIGHT * GFX_WIDTH];
static SDL_Texture *_framebuffer;
static SDL_Window *_window;
static SDL_Renderer *_renderer;

void platform_init(void)
{
	/* Init SDL */
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error initializing SDL; SDL_Init: %s\n",
			SDL_GetError());
		exit(1);
	}

	/* Create SDL_Window */
	if(!(_window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GFX_WIDTH, GFX_HEIGHT, 0)))
	{
		printf("Error creating SDL_Window: %s\n",
			SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	/* Create SDL_Renderer */
	if(!(_renderer = SDL_CreateRenderer
		(_window, -1, SDL_RENDERER_ACCELERATED)))
	{
		printf("Error creating SDL_Renderer: %s\n",
			SDL_GetError());
		SDL_DestroyWindow(_window);
		SDL_Quit();
		exit(1);
	}

	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	_framebuffer = SDL_CreateTexture(_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		GFX_WIDTH, GFX_HEIGHT);
}

void platform_destroy(void)
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void platform_palette(uint8_t *chg)
{
	_palette = chg;
}

static inline uint32_t g_color(int r, int g, int b)
{
	return b | (g << 8) | (r << 16);
}

void platform_blit(uint8_t *screen)
{
	for(int y = 0; y < 200; ++y)
	{
		for(int x = 0; x < 320; ++x)
		{
			uint8_t *color = &_palette[3 * screen[y * 320 + x]];
			uint32_t c = g_color(color[0], color[1], color[2]);
			_pixels[2*y * GFX_WIDTH + 2*x] = c;
			_pixels[2*y * GFX_WIDTH + 2*x + 1] = c;
			_pixels[(2 * y + 1) * GFX_WIDTH + 2*x] = c;
			_pixels[(2 * y + 1) * GFX_WIDTH + 2*x + 1] = c;
		}
	}

	SDL_UpdateTexture(_framebuffer, NULL, _pixels, GFX_WIDTH * sizeof(uint32_t));
	SDL_RenderClear(_renderer);
	SDL_RenderCopy(_renderer, _framebuffer, NULL, NULL);
	SDL_RenderPresent(_renderer);
}
