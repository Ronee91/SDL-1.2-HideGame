// Файл для библиотек и общих переменных
#include <stdio.h>
#include <assert.h>
#include <emscripten.h>
#include <stdint.h>
#include <math.h>
#include <SDL_ttf.h>

#define SCREEN_SIZE 600
#define ZOMBIE_COUNT 3

int g_intIgnorPress = 0;
int g_intHalScreenfSize = SCREEN_SIZE / 2;
int g_intScore = 0;

SDL_Surface *g_pScreen;
unsigned int *g_pPixels;
// Переменные цветов
Uint32 g_u32BorderColor = 0;
Uint32 g_u32FillColor = 0;
Uint32 g_u32PlayerColor = 0;
Uint32 g_u32PrizeColor = 0;
// Типы столкновений
enum ImpactType
{
    IMPACT_NONE,
    IMPACT_BORDER,
    IMPACT_FILL,
    IMPACT_PLAYER,
    IMPACT_PRIZE
};

int g_intMaxSpeed = 20; // Максимальная скорость игрока
int g_intActorSize = 6; // Размер актеров
int g_intHalfActorSize = g_intActorSize / 2;