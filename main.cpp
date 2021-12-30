// Основной файл, собирать от сюда
// команда сборки emcc main.cpp -o main.html --preload-file map.txt -Oz
// Файл карты должен лежать в корне

#include "global.cpp"
#include "file.cpp"
#include "render.cpp"

/**
 * @brief Отобразить очки
 * 
 */
void showScore()
{
	SDL_Rect rect = {0, SCREEN_SIZE - 33, 100, 30};
	TTF_Init();
	TTF_Font *font = TTF_OpenFont("Arial", 16);
	std::string l_sScore = "Score: " + std::to_string(g_intScore);
	SDL_Surface *text = TTF_RenderText_Solid(font, l_sScore.c_str(), {0xff, 0xff, 0x00, 0xff});
	SDL_FillRect(g_pScreen, &rect, SDL_MapRGB(g_pScreen->format, 0, 0, 0));
	SDL_BlitSurface(text, NULL, g_pScreen, &rect);
	SDL_FreeSurface(text);
	TTF_CloseFont(font);
}
/**
 * @brief Отобразить скорость
 * 
 */
void showSpeed()
{
	SDL_Rect rect = {100, SCREEN_SIZE - 33, 200, 30};
	TTF_Init();
	TTF_Font *font = TTF_OpenFont("Arial", 16);
	std::string l_sScore = "Speed (Z X): " + std::to_string(g_intMaxSpeed);
	SDL_Surface *text = TTF_RenderText_Solid(font, l_sScore.c_str(), {0xff, 0xff, 0x00, 0xff});
	SDL_FillRect(g_pScreen, &rect, SDL_MapRGB(g_pScreen->format, 0, 0, 0));
	SDL_BlitSurface(text, NULL, g_pScreen, &rect);
	SDL_FreeSurface(text);
	TTF_CloseFont(font);
}
/**
 * @brief Отобразить размер
 * 
 */
void showSize()
{
	SDL_Rect rect = {300, SCREEN_SIZE - 33, 200, 30};
	TTF_Init();
	TTF_Font *font = TTF_OpenFont("Arial", 16);
	std::string l_sScore = "Size (C V): " + std::to_string(g_intActorSize);
	SDL_Surface *text = TTF_RenderText_Solid(font, l_sScore.c_str(), {0xff, 0xff, 0x00, 0xff});
	SDL_FillRect(g_pScreen, &rect, SDL_MapRGB(g_pScreen->format, 0, 0, 0));
	SDL_BlitSurface(text, NULL, g_pScreen, &rect);
	SDL_FreeSurface(text);
	TTF_CloseFont(font);
}

#include "actors.cpp"

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	g_pScreen = SDL_SetVideoMode(SCREEN_SIZE, SCREEN_SIZE, 32, SDL_HWSURFACE);
	// Инициализуем цвета
	g_u32BorderColor = SDL_MapRGB(g_pScreen->format, 255, 255, 255);
	g_u32FillColor = SDL_MapRGB(g_pScreen->format, 0, 0, 255);
	g_u32PlayerColor = SDL_MapRGB(g_pScreen->format, 255, 255, 0);
	g_u32PrizeColor = SDL_MapRGB(g_pScreen->format, 0, 255, 0);

	SDL_LockSurface(g_pScreen);
	g_pPixels = (unsigned int *)g_pScreen->pixels;
	// Читаем файл с картой
	PoligonsCoordinates l_vFiguresCoordinates = readFile();
	// Инициализуем карту по точкам
	initMap(g_pPixels, l_vFiguresCoordinates);
	// Инициализуем актреров
	initActors();
	SDL_UnlockSurface(g_pScreen);
	showScore();
	showSpeed();
	showSize();
	// Цикл работы
	emscripten_set_main_loop(mainLoop, 0, 0);
	return 0;
}