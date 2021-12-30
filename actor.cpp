// Файл с логикой поведения всех актеров
// TODO Разбить на подклассы, или нет? жирненький файл получился
#include <stdlib.h>
#include <SDL/SDL.h>

#define MEMORY 60      // Память зомби о последней точки где видел игрока
#define FIND_CD 15     // Отсрочка поиска игрока
#define FIND_WAY_CD 25 // Отсрочка поиска пути
// Типы актеров
enum ActorType
{
    PLAYER,
    PRIZE,
    ZOMBIE
};
struct position
{
    int x;
    int y;
    int w;
    int h;
};
// Клас для описания поведения всех актеров
class Actor
{
private:
    void growSpeed(int *in_intSpeed, bool in_bPress);
    void setStartPosition(bool in_bIgnoreX);
    void setColor();
    void setWay();
    ImpactType checkColorPosition(position in_oPosition);

    position m_oActorPosition; // Расположение актера
    Uint32 m_u32Color = 0;     // Цвет актера (берётся из роли)
    int m_intSpeedL = 0;       // Скорость движения влево
    int m_intSpeedR = 0;       // Скорость движения вправо
    int m_intSpeedU = 0;       // Скорость движения в верх
    int m_intSpeedD = 0;       // Скорость движения вниз

    int m_intX = 0;                         // Центр актера по оси Х
    int m_intY = 0;                         // Центр актера по оси Y
    bool m_bVisible = true;                 // Видимость актера для игрока
    mapPoint targetPoint{0, 0};             // Точка направления для зомби
    int m_intWayMemory = 0;                 // Отсчет забывания
    int m_intFindCoolDown = FIND_CD;        // Отсчет КД поиска игрока
    int m_intFindWayCoolDown = FIND_WAY_CD; // Отсчет КД поиска пути

public:
    Actor(ActorType in_eType);
    ~Actor();

    void setRandomWay();
    void setSpeed(bool in_bPressL, bool in_bPressR, bool in_bPressU, bool in_bPressD);
    void moveActor();
    void newStartPosition(bool in_bClear);
    void redrawPosition(bool in_bClear);
    void clearPosition();
    mapPoint getCenterPoint();

    ActorType m_eType = PLAYER;
};
/**
 * @brief Construct a new Actor:: Actor object
 *  Конструктор актера
 * @param in_eType - тип игрока
 */
Actor::Actor(ActorType in_eType)
{
    m_eType = in_eType;
    setColor();
    newStartPosition(false);
}

/**
 * @brief Destroy the Actor:: Actor object
 * Деструктор актера
 */
Actor::~Actor()
{
}

/**
 * @brief Поиск пути для зомби
 * 
 */
void Actor::setWay()
{
    if (targetPoint.x == 0 && targetPoint.y == 0)
    {
        // Если нет точки где видел игрока и память закончилась
        if (m_intWayMemory == MEMORY)
        {
            // Идем в случайную сторону
            setRandomWay();
            m_intWayMemory = 0;
            m_intFindWayCoolDown = FIND_WAY_CD;
        }
    }
    else
    {
        if (m_intX == targetPoint.x && m_intY == targetPoint.y)
        {
            // Если дошли до точки где видели игрока, забываем точку, сбрасываем КД
            targetPoint.x = 0;
            targetPoint.y = 0;
            m_intWayMemory = MEMORY;
            m_intFindWayCoolDown = FIND_WAY_CD;
        }
        else
        {
            if (m_intFindWayCoolDown == FIND_WAY_CD)
            {
                // Ищем путь до игрока
                m_intFindWayCoolDown = 0;
                int l_intDifX = abs(m_intX - targetPoint.x);
                int l_intDifY = abs(m_intY - targetPoint.y);
                int l_intSpeedX = g_intScore;
                int l_intSpeedY = g_intScore;
                if (l_intDifX > l_intDifY)
                {
                    float l_fSpeed = float(l_intDifY) / float(l_intDifX) * g_intScore;
                    l_intSpeedY = (int)l_fSpeed;
                }
                else
                {
                    float l_fSpeed = float(l_intDifX) / float(l_intDifY) * g_intScore;
                    l_intSpeedX = (int)l_fSpeed;
                }
                if (m_intX == targetPoint.x)
                {
                    m_intSpeedL = 0;
                    m_intSpeedR = 0;
                }
                else if (m_intX > targetPoint.x)
                {
                    m_intSpeedL = l_intSpeedX;
                    m_intSpeedR = 0;
                }
                else
                {
                    m_intSpeedL = 0;
                    m_intSpeedR = l_intSpeedX;
                }
                if (m_intY == targetPoint.y)
                {
                    m_intSpeedU = 0;
                    m_intSpeedD = 0;
                }
                else if (m_intY > targetPoint.y)
                {
                    m_intSpeedU = l_intSpeedY;
                    m_intSpeedD = 0;
                }
                else
                {
                    m_intSpeedU = 0;
                    m_intSpeedD = l_intSpeedY;
                }
            }
            else
            {
                m_intFindWayCoolDown++;
            }
        }
    }
}

/**
 * @brief Поиск случайного направления для движения
 * 
 */
void Actor::setRandomWay()
{
    int l_intRand = rand() % 8;
    m_intSpeedU = 0;
    m_intSpeedR = 0;
    m_intSpeedD = 0;
    m_intSpeedL = 0;
    switch (l_intRand)
    {
    case 0:
        m_intSpeedU = g_intScore;
        break;
    case 1:
        m_intSpeedU = g_intScore;
        m_intSpeedR = g_intScore;
        break;
    case 2:
        m_intSpeedR = g_intScore;
        break;
    case 3:
        m_intSpeedD = g_intScore;
        m_intSpeedR = g_intScore;
        break;
    case 4:
        m_intSpeedD = g_intScore;
        break;
    case 5:
        m_intSpeedD = g_intScore;
        m_intSpeedL = g_intScore;
        break;
    case 6:
        m_intSpeedL = g_intScore;
        break;
    case 7:
        m_intSpeedU = g_intScore;
        m_intSpeedL = g_intScore;
        break;
    }
}

/**
 * @brief Получить центральную точку актера
 * 
 * @return mapPoint - объект с координатами
 */
mapPoint Actor::getCenterPoint()
{
    mapPoint l_oResult = {m_intX, m_intY};
    return l_oResult;
}
/**
 * @brief Очистка предыдущего положения, стирание с экрана
 * 
 */
void Actor::clearPosition()
{
    SDL_LockSurface(g_pScreen);
    drawActor(g_pPixels, m_oActorPosition.x, m_oActorPosition.y, g_intActorSize, 0x00000000);
    SDL_UnlockSurface(g_pScreen);
}
/**
 * @brief Перерисовка актера по его положению
 * 
 * @param in_bClear - флаг чистить ли предыдущее местоположение
 */
void Actor::redrawPosition(bool in_bClear)
{
    if (in_bClear)
    {
        // Чистим предыдущее местоположение
        clearPosition();
    }
    // Записываем положение
    m_oActorPosition = {m_intX - g_intHalfActorSize, m_intY - g_intHalfActorSize, m_intX + g_intHalfActorSize, m_intY + g_intHalfActorSize};
    // Отрисовываем
    setStartPosition(false);
}

/**
 * @brief Определить новую стартовую позицию
 * 
 * @param in_bClear - флаг чистить ли предыдущее местоположение
 */
void Actor::newStartPosition(bool in_bClear)
{
    // Для игрока позиция - центр карты
    if (m_eType == PLAYER)
    {
        m_intX = g_intHalScreenfSize;
        m_intY = g_intHalScreenfSize;
    }
    else
    {
        // Случайное место появления
        // TODO Магические числа чтобы ограничить появление картой
        int l_intMax = SCREEN_SIZE * 0.6;
        m_intX = (rand() % l_intMax) + 30;
        m_intY = (rand() % l_intMax) + 30;
        if (m_eType == ZOMBIE)
        {
            // Рандомим в какую сторону движется зомби
            setRandomWay();
        }
    }
    redrawPosition(in_bClear);
}

/**
 * @brief Установка цвета актеру в соответствии его типа
 * 
 */
void Actor::setColor()
{
    switch (m_eType)
    {
    case PLAYER:
        m_u32Color = g_u32PlayerColor;
        break;
    case PRIZE:
        m_u32Color = g_u32PrizeColor;
        break;
    case ZOMBIE:
        m_u32Color = SDL_MapRGB(g_pScreen->format, 255, 0, 0);
        break;
    }
}

/**
 * @brief Проверка цветов на столкновения
 * 
 * @param in_oPosition - Позиция для проверки
 * @return ImpactType - тип столкновения
 */
ImpactType Actor::checkColorPosition(position in_oPosition)
{
    ImpactType l_eResult = IMPACT_NONE;
    // По очереди проверяем каждую сторону на предмет столкновения
    ImpactType l_eCheckX = checkColorsX(g_pPixels, in_oPosition.x, in_oPosition.w, in_oPosition.y, m_eType == PLAYER);
    if (l_eCheckX != IMPACT_NONE)
    {
        l_eResult = l_eCheckX;
    }
    else
    {
        ImpactType l_eCheckY = checkColorsY(g_pPixels, in_oPosition.y, in_oPosition.h, in_oPosition.x, m_eType == PLAYER);
        if (l_eCheckY != IMPACT_NONE)
        {
            l_eResult = l_eCheckY;
        }
        else
        {
            ImpactType l_eCheckW = checkColorsX(g_pPixels, in_oPosition.x, in_oPosition.w, in_oPosition.h, m_eType == PLAYER);
            if (l_eCheckW != IMPACT_NONE)
            {
                l_eResult = l_eCheckW;
            }
            else
            {
                l_eResult = checkColorsY(g_pPixels, in_oPosition.y, in_oPosition.h, in_oPosition.w, m_eType == PLAYER);
            }
        }
    }
    return l_eResult;
}

/**
 * @brief Отрисовка стартовой позиции с проверкой на попадание в карту (рекурсивная функция)
 * Поиск осущетвляется только по кресту
 * @param in_bIgnoreX - игнорировать вертикаль (в случае если не нашли подходящей точки и исчем по горизонтали)
 */
void Actor::setStartPosition(bool in_bIgnoreX)
{
    // Проверяем позицию на столкновения
    ImpactType l_eCheck = checkColorPosition(m_oActorPosition);
    if (l_eCheck == IMPACT_NONE)
    {
        // Отрисовка по позиции если нет столкновений
        m_intX = m_oActorPosition.x + g_intHalfActorSize;
        m_intY = m_oActorPosition.y + g_intHalfActorSize;
        SDL_LockSurface(g_pScreen);
        drawActor(g_pPixels, m_oActorPosition.x, m_oActorPosition.y, g_intActorSize, m_u32Color);
        SDL_UnlockSurface(g_pScreen);
    }
    else
    {
        if (!in_bIgnoreX)
        {
            // Ищем точку по вертикали
            bool l_bignoreX = in_bIgnoreX;
            if (m_oActorPosition.x < m_intX - g_intHalfActorSize)
            {
                m_oActorPosition.x = m_oActorPosition.x - 20;
                m_oActorPosition.w = m_oActorPosition.w - 20;
            }
            else
            {
                m_oActorPosition.x = m_oActorPosition.x + 20;
                m_oActorPosition.w = m_oActorPosition.w + 20;
            }
            if (m_oActorPosition.w > SCREEN_SIZE)
            {
                m_oActorPosition = {m_intX - g_intHalfActorSize - 20, m_intY - g_intHalfActorSize, m_intX + g_intHalfActorSize - 20, m_intY + g_intHalfActorSize};
            }
            if (m_oActorPosition.x < 0)
            {
                m_oActorPosition.x = m_intX - g_intHalfActorSize;
                m_oActorPosition.w = m_intX + g_intHalfActorSize;
                l_bignoreX = true;
            }
            setStartPosition(l_bignoreX);
        }
        else
        {
            // Ищем точку по горизонтали
            if (m_oActorPosition.y < m_intY - g_intHalfActorSize)
            {
                m_oActorPosition.y = m_oActorPosition.y - 20;
                m_oActorPosition.h = m_oActorPosition.h - 20;
            }
            else
            {
                m_oActorPosition.y = m_oActorPosition.y + 20;
                m_oActorPosition.h = m_oActorPosition.h + 20;
            }
            if (m_oActorPosition.h > SCREEN_SIZE)
            {
                m_oActorPosition = {m_intX - g_intHalfActorSize, m_intY - g_intHalfActorSize - 20, m_intX + g_intHalfActorSize, m_intY + g_intHalfActorSize - 20};
            }
            if (m_oActorPosition.y < 0)
            {
                // Проверили всю крестовину
                printf("На крестовине нет места для актера \n");
            }
            else
            {
                setStartPosition(in_bIgnoreX);
            }
        }
    }
}

/**
 * @brief Движения актера
 * 
 */
void Actor::moveActor()
{
    // Считаем шаг движения в зависимости от скоростей
    int l_intMoveX = m_intSpeedR - m_intSpeedL;
    int l_intMoveY = m_intSpeedD - m_intSpeedU;
    if (l_intMoveX || l_intMoveY)
    {
        if (m_eType == PLAYER)
        {
            l_intMoveX = l_intMoveX / 10;
            l_intMoveY = l_intMoveY / 10;
        }
        else
        {
            l_intMoveX = l_intMoveX / 2;
            l_intMoveY = l_intMoveY / 2;
        }
        m_intX = m_intX + l_intMoveX;
        m_intY = m_intY + l_intMoveY;
        // Получаем новую позицию
        position l_pNewPlayerPosition{m_oActorPosition.x + l_intMoveX, m_oActorPosition.y + l_intMoveY, m_oActorPosition.w + l_intMoveX, m_oActorPosition.h + l_intMoveY};
        if (l_pNewPlayerPosition.x >= 0 && l_pNewPlayerPosition.y >= 0 && l_pNewPlayerPosition.w < SCREEN_SIZE && l_pNewPlayerPosition.h < SCREEN_SIZE)
        {
            // Проверяем новую позиция на стоклновения
            ImpactType l_eCheck = checkColorPosition(l_pNewPlayerPosition);
            if (l_eCheck == IMPACT_NONE)
            {
                // Если нет столкновений Ставим позицию и рисуем актера если он видимый
                SDL_LockSurface(g_pScreen);
                drawActor(g_pPixels, m_oActorPosition.x, m_oActorPosition.y, g_intActorSize, 0x00000000);
                m_oActorPosition = l_pNewPlayerPosition;
                if (m_bVisible)
                {
                    drawActor(g_pPixels, m_oActorPosition.x, m_oActorPosition.y, g_intActorSize, m_u32Color);
                }
                SDL_UnlockSurface(g_pScreen);
            }
            else
            {
                if (l_eCheck == IMPACT_PLAYER)
                {
                    // Зомби догнал игрока, телепортируем в рандомную позицию
                    targetPoint.x = 0;
                    targetPoint.y = 0;
                    newStartPosition(true);
                    g_intScore = g_intScore / 2;
                    showScore();
                }
                else
                {
                    if (m_eType == PLAYER && l_eCheck == IMPACT_PRIZE)
                    {
                        // Игрок добрался до приза
                        getPrize();
                    }
                    else
                    {
                        if (m_eType == PLAYER)
                        {
                            // Если игрок ударился об стену, кантузия, временная потеря управления (так приятней чем бится в одну и туже точку 3 раза пока держиш кнопку)
                            g_intIgnorPress = 10;
                        }
                        else
                        {
                            // Если зомби ударился об стену при приследовании, всё забыл поперся по своим делам
                            targetPoint.x = 0;
                            targetPoint.y = 0;
                            m_intWayMemory = MEMORY;
                            m_intFindWayCoolDown = FIND_WAY_CD;
                        }
                        // Полурандомное отскакивание от стен
                        m_intSpeedR = -m_intSpeedR;
                        m_intSpeedL = -m_intSpeedL;
                        m_intSpeedU = -m_intSpeedU;
                        m_intSpeedD = -m_intSpeedD;
                        if ((m_intSpeedR + m_intSpeedL != 0) && (m_intSpeedU + m_intSpeedD != 0))
                        {
                            if (rand() % 2)
                            {
                                m_intSpeedR = 0;
                                m_intSpeedL = 0;
                            }
                            else
                            {
                                m_intSpeedU = 0;
                                m_intSpeedD = 0;
                            }
                        }
                        else
                        {
                            int l_intSpeed = m_eType == PLAYER ? g_intMaxSpeed * 0.7 : g_intScore;
                            if (m_intSpeedR + m_intSpeedL == 0)
                            {
                                if (rand() % 2)
                                {
                                    m_intSpeedR = l_intSpeed;
                                }
                                else
                                {
                                    m_intSpeedL = l_intSpeed;
                                }
                            }
                            if (m_intSpeedU + m_intSpeedD == 0)
                            {
                                if (rand() % 2)
                                {
                                    m_intSpeedU = l_intSpeed;
                                }
                                else
                                {
                                    m_intSpeedD = l_intSpeed;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (m_eType == ZOMBIE)
    {
        // Проверяем видит ли зомби игрока
        mapPoint l_oPlayerCenter = getPlayerCenterPoint();
        ImpactType l_eCheck = checkLine(g_pPixels, m_intX, m_intY, l_oPlayerCenter.x, l_oPlayerCenter.y);
        if (l_eCheck == IMPACT_NONE)
        {
            // КД для нахождения, с вниманием у зомби не очень
            if (m_intFindCoolDown == FIND_CD)
            {
                m_intFindCoolDown = 0;
                m_bVisible = true;
                targetPoint = l_oPlayerCenter;
                m_intWayMemory = 0;
            }
            else
            {
                m_intFindCoolDown++;
            }
        }
        else
        {
            // Забывание нахождения игрока
            if (targetPoint.x != 0 && targetPoint.y != 0)
            {
                if (m_intWayMemory < MEMORY)
                {
                    m_intWayMemory++;
                }
            }
            m_bVisible = false;
        }
        // Поиск пути зомби до игрока
        setWay();
    }
}

/**
 * @brief Установка скорости
 * 
 * @param in_bPressL - нажатие клавиши влево
 * @param in_bPressR - нажатие клавиши вправо
 * @param in_bPressU - нажатие клавиши в верх
 * @param in_bPressD - нажатие клавиши вниз
 */
void Actor::setSpeed(bool in_bPressL, bool in_bPressR, bool in_bPressU, bool in_bPressD)
{
    growSpeed(&m_intSpeedL, in_bPressL);
    growSpeed(&m_intSpeedR, in_bPressR);
    growSpeed(&m_intSpeedU, in_bPressU);
    growSpeed(&m_intSpeedD, in_bPressD);
}

// Повышение скорости по направлению
void Actor::growSpeed(int *in_intSpeed, bool in_bPress)
{
    // Повышаем скорость направления если клавиши зажаты и не привышают максимальную скорость
    if (in_bPress && *in_intSpeed < g_intMaxSpeed)
    {
        *in_intSpeed = *in_intSpeed + 3;
    }
    // Если клавиша не нажата понижаем не нулевую скорость
    if (!in_bPress && *in_intSpeed != 0)
    {
        if (*in_intSpeed > 0)
        {
            *in_intSpeed = *in_intSpeed - 1;
        }
        else
        {
            *in_intSpeed = *in_intSpeed + 1;
        }
    }
}