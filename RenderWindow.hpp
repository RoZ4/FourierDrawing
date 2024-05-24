#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>

class RenderWindow {
public:
	SDL_Texture* texture;
	Uint32* pixels;
	RenderWindow(const char* p_title, int p_w, int p_h);
	void drawRect(int r, int g, int b, int a, int32_t x, int32_t y, int32_t w, int32_t h);
	void drawCircle(int r, int g, int b, int a, int32_t centreX, int32_t centreY, int32_t radius);
	void drawLine(int r, int g, int b, int a, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
	void drawPoint(int r, int g, int b, int a, int32_t x0, int32_t y0);
	void drawLines(int r, int g, int b, int a, SDL_FPoint* points, int number);

	void cleanUp();
	void clear();
	void render(SDL_Texture* p_tex);
	void display();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	
};