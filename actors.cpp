// Файл Взаимодействия между актерами

void getPrize();
mapPoint getPlayerCenterPoint();
#include "actor.cpp"
Actor *g_aActors[ZOMBIE_COUNT + 2]; // Массив указателей на всех актеров, в 0 приз, в 1 игрок

/**
 * @brief Игрок добрался до приза
 * 
 */
void getPrize()
{
    g_intScore = g_intScore + 1;
    // Перемещаем приз в новую позицию
    g_aActors[0]->newStartPosition(true);
    showScore();
};

/**
 * @brief Получение центральной точки игрока (нужно для преследования)
 * 
 * @return mapPoint - объект с координатами
 */
mapPoint getPlayerCenterPoint()
{
    mapPoint l_oResult = {0, 0};
    if (g_aActors[1])
    {
        l_oResult = g_aActors[1]->getCenterPoint();
    }
    return l_oResult;
};

bool g_bPressL = false;
bool g_bPressR = false;
bool g_bPressU = false;
bool g_bPressD = false;

/**
 * @brief Установка флагов нажатости клавиш (используется при нажатии и отжатии)
 * 
 * @param in_key - ключ клавиши
 * @param in_bValue - значение для установки
 */
void setButtonPressFlags(SDLKey in_key, bool in_bValue)
{
    switch (in_key)
    {
    case SDLK_LEFT:
    case SDLK_a:
        g_bPressL = in_bValue;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        g_bPressR = in_bValue;
        break;
    case SDLK_UP:
    case SDLK_w:
        g_bPressU = in_bValue;
        break;
    case SDLK_DOWN:
    case SDLK_s:
        g_bPressD = in_bValue;
        break;
    }
}

void setActorsNewSize(int in_intNewSize)
{
    if (in_intNewSize > 0 && in_intNewSize < 40)
    {
        // Стираем отображение всех актеров
        for (int i = 0; i < ZOMBIE_COUNT + 2; i++)
        {
            g_aActors[i]->clearPosition();
        }
        g_intActorSize = in_intNewSize;
        g_intHalfActorSize = g_intActorSize / 2;
        showSize();
        // Перерисовка всех актеров
        for (int i = 0; i < ZOMBIE_COUNT + 2; i++)
        {
            g_aActors[i]->redrawPosition(false);
        }
    }
}
/**
 * @brief Цикл работы, отлавливание клавиш и движения актеров
 * 
 */
void mainLoop()
{
    SDL_Event event;
    // Проверяем клавиши на события
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:

            switch (event.key.keysym.sym)
            {
            case SDLK_z:
                if (g_intMaxSpeed != 10)
                {
                    g_intMaxSpeed = g_intMaxSpeed - 5;
                }
                showSpeed();
                break;
            case SDLK_x:
                if (g_intMaxSpeed < 100)
                {
                    g_intMaxSpeed = g_intMaxSpeed + 5;
                }
                showSpeed();
                break;
            case SDLK_c:
                setActorsNewSize(g_intActorSize - 2);
                break;
            case SDLK_v:
                setActorsNewSize(g_intActorSize + 2);
                break;
            }
            // Ставим флаг нажатия клавиши
            setButtonPressFlags(event.key.keysym.sym, true);
            break;
        case SDL_KEYUP:
            // Снимаем флаг нажатия клавиши
            setButtonPressFlags(event.key.keysym.sym, false);
            break;
        }
    }
    // Передвигаем актеров
    for (int i = 0; i < ZOMBIE_COUNT + 2; i++)
    {
        if (g_aActors[i]->m_eType == PLAYER)
        {
            // Устанавливаем скорость игрока
            if (g_intIgnorPress > 0)
            {
                g_intIgnorPress--;
            }
            else
            {
                g_aActors[i]->setSpeed(g_bPressL, g_bPressR, g_bPressU, g_bPressD);
            }
        }
        // Передвигаем всех актеров
        g_aActors[i]->moveActor();
    }
}

/**
 * @brief Инициализация актеров
 * 
 */
void initActors()
{
    srand(time(NULL));
    g_aActors[0] = new Actor(PRIZE);
    g_aActors[1] = new Actor(PLAYER);
    for (int i = 0; i < ZOMBIE_COUNT; i++)
    {
        g_aActors[i + 2] = new Actor(ZOMBIE);
    }
}