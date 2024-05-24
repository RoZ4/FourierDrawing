#include <SDL.h>
#include <iostream>
#include <math.h>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) : window(NULL), renderer(NULL), texture(NULL), pixels(new Uint32[p_w * p_h]){
	
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, p_h, p_w);
	
	//Uint32* pixels = new Uint32[p_w * p_h];
	memset(pixels, 156, p_w * p_h * sizeof(Uint32));
}

void RenderWindow::drawRect(int r, int g, int b, int a, int32_t x, int32_t y, int32_t w, int32_t h) {
	SDL_Rect rect{};
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::drawCircle(int r, int g, int b, int a, int32_t centreX, int32_t centreY, int32_t radius) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	const int diameter = (radius * 2);
	int x = (radius - 1);
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = (tx - diameter);
	while (x >= y) {
		SDL_RenderDrawPointF(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPointF(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPointF(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPointF(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPointF(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPointF(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPointF(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPointF(renderer, centreX - y, centreY + x);
		if (error <= 0) {
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0) {
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

void RenderWindow::drawLine(int r, int g, int b, int a, int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_RenderDrawLineF(renderer, x0, y0, x1, y1);
}

void RenderWindow::drawPoint(int r, int g, int b, int a, int32_t x0, int32_t y0) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_RenderDrawPointF(renderer, x0, y0);
}

void RenderWindow::drawLines(int r, int g, int b, int a, SDL_FPoint* points, int number) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_RenderDrawLinesF(renderer, points, number);
}

void RenderWindow::cleanUp() {
	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(SDL_Texture* p_tex) {
	SDL_RenderCopy(renderer, p_tex, NULL, NULL);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}