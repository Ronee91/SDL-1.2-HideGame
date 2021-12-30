// Файл для описания отрисовки
/**
 * @brief Нарисовать пиксель
 * 
 * @param in_pPixelBuffer - указатель на пиксели
 * @param in_intX - координата по оси Х
 * @param in_intY - координата по оси Y
 * @param in_u32Color - цвет
 */
void drawPixel(uint32_t *in_pPixelBuffer, int in_intX, int in_intY, uint32_t in_u32Color)
{
	if (!(in_intX < 0 || in_intY < 0 || in_intX >= SCREEN_SIZE || in_intY >= SCREEN_SIZE))
	{
		uint32_t l_u32Position = in_intY * SCREEN_SIZE + in_intX;
		in_pPixelBuffer[l_u32Position] = in_u32Color;
	}
}

/**
 * @brief Нарисовать линию
 * 
 * @param in_pPixelBuffer - указатель на пиксели
 * @param in_intX1 - координата по оси Х первой точки
 * @param in_intY1 - координата по оси Y первой точки
 * @param in_intX2 - координата по оси Х второй точки
 * @param in_intY2 - координата по оси Y второй точки
 * @param in_u32Color - цвет
 */
void drawLine(uint32_t *in_pPixelBuffer, int in_intX1, int in_intY1, int in_intX2, int in_intY2, uint32_t in_u32Color)
{
	// Ставим первый пиксель
	drawPixel(in_pPixelBuffer, in_intX1, in_intY1, in_u32Color);
	// Рисуем линии с лева на право
	if (in_intX1 > in_intX2)
	{
		int l_intTempX = in_intX1;
		int l_intTempY = in_intY1;

		in_intX1 = in_intX2;
		in_intY1 = in_intY2;

		in_intX2 = l_intTempX;
		in_intY2 = l_intTempY;
	}

	int l_intDX = in_intX2 - in_intX1;
	int l_intDY = in_intY2 - in_intY1;
	// Длина линии больше по оси X
	if (l_intDX >= fabs(l_intDY))
	{
		float l_fSlope = (float)l_intDY / l_intDX;
		// Линия с верху вниз
		if (in_intY1 <= in_intY2)
		{
			float l_fIdealY = in_intY1 + l_fSlope;
			int y = (int)round(l_fIdealY);
			float l_fError = l_fIdealY - y;
			int i = 0;
			// Обходим все значени¤ X
			for (i = 1; i <= l_intDX; i++)
			{
				int x = in_intX1 + i;
				drawPixel(in_pPixelBuffer, x, y, in_u32Color);
				l_fError += l_fSlope;
				if (l_fError >= 0.5)
				{
					y++;
					l_fError -= 1;
				}
			}
		}
		// Линия с низу в верх
		if (in_intY1 > in_intY2)
		{
			float l_fIdealY = in_intY1 + l_fSlope;
			int y = (int)round(l_fIdealY);
			float l_fError = l_fIdealY - y;
			int i = 0;
			// Обходим все значени¤ X
			for (i = 1; i <= l_intDX; i++)
			{
				int x = in_intX1 + i;
				drawPixel(in_pPixelBuffer, x, y, in_u32Color);
				l_fError += l_fSlope;
				if (l_fError <= -0.5)
				{
					y--;
					l_fError += 1;
				}
			}
		}
	}
	// Длина линии больше по оси Y
	if (fabs(l_intDY) > l_intDX)
	{
		float l_fSlope = (float)l_intDX / l_intDY;
		// Линия с верху вниз
		if (in_intY1 < in_intY2)
		{
			float in_fIdealX = in_intX1 + l_fSlope;
			int x = (int)round(in_fIdealX);
			float l_fError = in_fIdealX - x;
			int i = 0;
			// Обходим все значени¤ Y
			for (i = 1; i <= l_intDY; i++)
			{
				int y = in_intY1 + i;
				drawPixel(in_pPixelBuffer, x, y, in_u32Color);
				l_fError += l_fSlope;
				if (l_fError >= 0.5)
				{
					x++;
					l_fError -= 1;
				}
			}
		}
		// Линия с низу в верх
		if (in_intY1 > in_intY2)
		{
			float in_fIdealX = in_intX1 - l_fSlope;
			int x = (int)round(in_fIdealX);
			float l_fError = in_fIdealX - x;
			int i = 0;
			// Обходим все значени¤ Y
			for (i = 1; i <= fabs(l_intDY); i++)
			{
				int y = in_intY1 - i;
				drawPixel(in_pPixelBuffer, x, y, in_u32Color);
				l_fError += l_fSlope;
				if (l_fError <= -0.5)
				{
					x++;
					l_fError += 1;
				}
			}
		}
	}
}

/**
 * @brief Проверить пиксель на столкновение по цвету
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_intX - координата по оси Х
 * @param in_intY - координата по оси Y
 * @param in_bIgnorePlayer - игнорировать столкновения с игроком (скорее всего сам игрок проверяет)
 * @return ImpactType 
 */
ImpactType checkColor(uint32_t *in_pPixelBuffer, int in_intX, int in_intY, bool in_bIgnorePlayer)
{
	ImpactType l_eResult = IMPACT_NONE;
	uint32_t l_u32Position = in_intY * SCREEN_SIZE + in_intX;
	if (in_pPixelBuffer[l_u32Position] == g_u32BorderColor)
	{
		l_eResult = IMPACT_BORDER;
	}
	else if (in_pPixelBuffer[l_u32Position] == g_u32FillColor)
	{
		l_eResult = IMPACT_FILL;
	}
	else if (!in_bIgnorePlayer && in_pPixelBuffer[l_u32Position] == g_u32PlayerColor)
	{
		l_eResult = IMPACT_PLAYER;
	}
	else if (in_pPixelBuffer[l_u32Position] == g_u32PrizeColor)
	{
		l_eResult = IMPACT_PRIZE;
	}
	return l_eResult;
}

/**
 * @brief Проверка вертикальной полосы на столкновение по цвету
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_intX1 - координата по оси Х
 * @param in_intX2 - вторая  координата по оси Х
 * @param in_intY  - координата по оси Y
 * @param in_bIgnorePlayer - игнорировать столкновения с игроком (скорее всего сам игрок проверяет)
 * @return ImpactType 
 */
ImpactType checkColorsX(uint32_t *in_pPixelBuffer, int in_intX1, int in_intX2, int in_intY, bool in_bIgnorePlayer)
{
	ImpactType l_eResult = IMPACT_NONE;
	for (int i = in_intX1; i < in_intX2 + 1; i++)
	{
		l_eResult = checkColor(in_pPixelBuffer, i, in_intY, in_bIgnorePlayer);
		if (l_eResult != IMPACT_NONE)
		{
			break;
		}
	}
	return l_eResult;
}

/**
 * @brief 
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_intY1 - координата по оси Y
 * @param in_intY2 - вторая координата по оси Y
 * @param in_intX  - координата по оси Х
 * @param in_bIgnorePlayer 
 * @return ImpactType 
 */
ImpactType checkColorsY(uint32_t *in_pPixelBuffer, int in_intY1, int in_intY2, int in_intX, bool in_bIgnorePlayer)
{
	ImpactType l_eResult = IMPACT_NONE;
	for (int i = in_intY1; i < in_intY2 + 1; i++)
	{
		l_eResult = checkColor(in_pPixelBuffer, in_intX, i, in_bIgnorePlayer);
		if (l_eResult != IMPACT_NONE)
		{
			break;
		}
	}
	return l_eResult;
}

/**
 * @brief Проверить пиксель по цвету, для заливки полигонов
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_intX - координата по оси Х
 * @param in_intY - координата по оси Y
 * @return true - не встретил границу или цвет заливки
 * @return false - встретил границу или цвет заливки
 */
bool checkFillColor(uint32_t *in_pPixelBuffer, int in_intX, int in_intY)
{
	uint32_t l_u32Position = in_intY * SCREEN_SIZE + in_intX;
	return in_pPixelBuffer[l_u32Position] != g_u32BorderColor && in_pPixelBuffer[l_u32Position] != g_u32FillColor;
}

/**
 * @brief Залить полигон (Рекурсивная функция)
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_intX - координата по оси Х
 * @param in_intY - координата по оси Y
 */
void fillPolygon(uint32_t *in_pPixelBuffer, int in_intX, int in_intY)
{
	// TODO не учитывает пересечения полигонов, при больших полигонах может не хватить памяти на рекурсию
	bool l_bRight = checkFillColor(in_pPixelBuffer, in_intX + 1, in_intY);
	bool l_bLeft = checkFillColor(in_pPixelBuffer, in_intX - 1, in_intY);
	bool l_bTop = checkFillColor(in_pPixelBuffer, in_intX, in_intY - 1);
	bool l_bBottom = checkFillColor(in_pPixelBuffer, in_intX, in_intY + 1);
	if (l_bRight)
	{
		drawPixel(in_pPixelBuffer, in_intX + 1, in_intY, g_u32FillColor);
		fillPolygon(in_pPixelBuffer, in_intX + 1, in_intY);
	}
	if (l_bLeft)
	{
		drawPixel(in_pPixelBuffer, in_intX - 1, in_intY, g_u32FillColor);
		fillPolygon(in_pPixelBuffer, in_intX - 1, in_intY);
	}
	if (l_bTop)
	{
		drawPixel(in_pPixelBuffer, in_intX, in_intY - 1, g_u32FillColor);
		fillPolygon(in_pPixelBuffer, in_intX, in_intY - 1);
	}
	if (l_bBottom)
	{
		drawPixel(in_pPixelBuffer, in_intX, in_intY + 1, g_u32FillColor);
		fillPolygon(in_pPixelBuffer, in_intX, in_intY + 1);
	}
}

/**
 * @brief Нарисовать актера
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_intX  - координата по оси Х
 * @param in_intY  - координата по оси Y
 * @param size - размер
 * @param in_u32Color -цвет
 */
void drawActor(uint32_t *in_pPixelBuffer, int in_intX, int in_intY, int size, uint32_t in_u32Color)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			drawPixel(in_pPixelBuffer, in_intX + i, in_intY + j, in_u32Color);
		}
	}
}

/**
 * @brief Вычеслить центр полигона (Для заливки полигона)
 * 
 * @param vertices точки полигона
 * @return mapPoint - координаты точки центра полигона
 */
mapPoint computeCentroid(PoligonPoints vertices)
{
	mapPoint l_oResult = {0, 0};
	double l_dSignedArea = 0.0;
	double l_dX = 0.0;	// Текущая вершина X
	double l_dY = 0.0;	// Текущая вершина Y
	double l_dX1 = 0.0; // Следующая вершина X
	double l_dX2 = 0.0; // Следующая вершина Y
	double l_dA = 0.0;	// Частично подписанная область

	// Для всех вершин кроме последней
	int i = 0;
	for (i = 0; i < vertices.size() - 1; ++i)
	{
		l_dX = vertices[i].x;
		l_dY = vertices[i].y;
		l_dX1 = vertices[i + 1].x;
		l_dX2 = vertices[i + 1].y;
		l_dA = l_dX * l_dX2 - l_dX1 * l_dY;
		l_dSignedArea += l_dA;
		l_oResult.x += (l_dX + l_dX1) * l_dA;
		l_oResult.y += (l_dY + l_dX2) * l_dA;
	}

	// Последняя вершина
	l_dX = vertices[i].x;
	l_dY = vertices[i].y;
	l_dX1 = vertices[0].x;
	l_dX2 = vertices[0].y;
	l_dA = l_dX * l_dX2 - l_dX1 * l_dY;
	l_dSignedArea += l_dA;
	l_oResult.x += (l_dX + l_dX1) * l_dA;
	l_oResult.y += (l_dY + l_dX2) * l_dA;

	l_dSignedArea *= 0.5;
	l_oResult.x /= (6.0 * l_dSignedArea);
	l_oResult.y /= (6.0 * l_dSignedArea);

	return l_oResult;
}

/**
 * @brief Получить точку по координатам, адаптация координаты карты под экран
 * 
 * @param in_intCoord - координата
 * @param in_fIndex - индекс соответствия карты и экрана
 * @return int - точка экрана
 */
int getPointCoord(int in_intCoord, float in_fIndex)
{
	int l_intResult = 0;
	if (in_intCoord > 0)
	{
		l_intResult = (in_intCoord * in_fIndex) + g_intHalScreenfSize;
	}
	else
	{
		l_intResult = g_intHalScreenfSize - (abs(in_intCoord) * in_fIndex);
	}
	return l_intResult;
}

/**
 * @brief Инициализация карты
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_vFiguresCoordinates - вектор полигонов из файла
 */
void initMap(uint32_t *in_pPixelBuffer, PoligonsCoordinates in_vFiguresCoordinates)
{
	drawLine(in_pPixelBuffer, 1, 1, SCREEN_SIZE - 2, 1, g_u32BorderColor);
	drawLine(in_pPixelBuffer, 1, 1, 1, SCREEN_SIZE - 2, g_u32BorderColor);
	drawLine(in_pPixelBuffer, SCREEN_SIZE - 2, 2, SCREEN_SIZE - 2, SCREEN_SIZE - 2, g_u32BorderColor);
	drawLine(in_pPixelBuffer, 2, SCREEN_SIZE - 2, SCREEN_SIZE - 2, SCREEN_SIZE - 2, g_u32BorderColor);

	float l_fMapIndex = (float)g_intHalScreenfSize / (float)g_intMaxPoint;
	for (PoligonsCoordinates::iterator figure = in_vFiguresCoordinates.begin(); figure != in_vFiguresCoordinates.end(); figure++)
	{
		// Адаптируем точки карты под размеры
		PoligonPoints l_vPoints;
		for (PoligonPoints::iterator it = figure->begin(); it != figure->end(); it++)
		{
			mapPoint l_oPoint{getPointCoord(it->x, l_fMapIndex), getPointCoord(it->y, l_fMapIndex)};
			l_vPoints.push_back(l_oPoint);
		}
		// Рисуем линии по адаптивированным точкам карты
		for (PoligonPoints::iterator it = l_vPoints.begin() + 1; it != l_vPoints.end(); it++)
		{
			auto prev = it - 1;
			drawLine(in_pPixelBuffer, prev->x, prev->y, it->x, it->y, g_u32BorderColor);
		}
		auto l_pLast = l_vPoints.end() - 1;
		drawLine(in_pPixelBuffer, l_pLast->x, l_pLast->y, l_vPoints.begin()->x, l_vPoints.begin()->y, g_u32BorderColor);
		mapPoint l_oCentroid = computeCentroid(l_vPoints);
		fillPolygon(in_pPixelBuffer, l_oCentroid.x, l_oCentroid.y);
	}
}

/**
 * @brief Проверить линию на столкновения (для выяснения видимости)
 * 
 * @param in_pPixelBuffer  - указатель на пиксели
 * @param in_intX1 - координата по оси Х первой точки
 * @param in_intY1 - координата по оси Y первой точки
 * @param in_intX2 - координата по оси Х второй точки
 * @param in_intY2 - координата по оси Y второй точки
 * @return ImpactType 
 */
ImpactType checkLine(uint32_t *in_pPixelBuffer, int in_intX1, int in_intY1, int in_intX2, int in_intY2)
{
	// Ставим первый пиксель
	ImpactType l_eResult = checkColor(in_pPixelBuffer, in_intX1, in_intY1, true);
	if (l_eResult != IMPACT_NONE)
	{
		return l_eResult;
	}
	// Рисуем линии с лева на право
	if (in_intX1 > in_intX2)
	{
		int l_intTempX = in_intX1;
		int l_intTempY = in_intY1;

		in_intX1 = in_intX2;
		in_intY1 = in_intY2;

		in_intX2 = l_intTempX;
		in_intY2 = l_intTempY;
	}

	int l_intDX = in_intX2 - in_intX1;
	int l_intDY = in_intY2 - in_intY1;
	// Длина линии больше по оси X
	if (l_intDX >= fabs(l_intDY))
	{
		float l_fSlope = (float)l_intDY / l_intDX;
		// Линия с верху вниз
		if (in_intY1 <= in_intY2)
		{
			float l_fIdealY = in_intY1 + l_fSlope;
			int y = (int)round(l_fIdealY);
			float l_fError = l_fIdealY - y;
			int i = 0;
			// Обходим все значения X
			for (i = 1; i <= l_intDX; i++)
			{
				int x = in_intX1 + i;
				ImpactType l_eResult = checkColor(in_pPixelBuffer, x, y, true);
				if (l_eResult != IMPACT_NONE)
				{
					return l_eResult;
				}
				l_fError += l_fSlope;
				if (l_fError >= 0.5)
				{
					y++;
					l_fError -= 1;
				}
			}
		}
		// Линия с низу в верх
		if (in_intY1 > in_intY2)
		{
			float l_fIdealY = in_intY1 + l_fSlope;
			int y = (int)round(l_fIdealY);
			float l_fError = l_fIdealY - y;
			int i = 0;
			// Обходим все значения X
			for (i = 1; i <= l_intDX; i++)
			{
				int x = in_intX1 + i;
				ImpactType l_eResult = checkColor(in_pPixelBuffer, x, y, true);
				if (l_eResult != IMPACT_NONE)
				{
					return l_eResult;
				}
				l_fError += l_fSlope;
				if (l_fError <= -0.5)
				{
					y--;
					l_fError += 1;
				}
			}
		}
	}
	// Длина линии больше по оси Y
	if (fabs(l_intDY) > l_intDX)
	{
		float l_fSlope = (float)l_intDX / l_intDY;
		// Линия с верху вниз
		if (in_intY1 < in_intY2)
		{
			float in_fIdealX = in_intX1 + l_fSlope;
			int x = (int)round(in_fIdealX);
			float l_fError = in_fIdealX - x;
			int i = 0;
			// Обходим все значения Y
			for (i = 1; i <= l_intDY; i++)
			{
				int y = in_intY1 + i;
				ImpactType l_eResult = checkColor(in_pPixelBuffer, x, y, true);
				if (l_eResult != IMPACT_NONE)
				{
					return l_eResult;
				}
				l_fError += l_fSlope;
				if (l_fError >= 0.5)
				{
					x++;
					l_fError -= 1;
				}
			}
		}
		// Линия с низу в верх
		if (in_intY1 > in_intY2)
		{
			float in_fIdealX = in_intX1 - l_fSlope;
			int x = (int)round(in_fIdealX);
			float l_fError = in_fIdealX - x;
			int i = 0;
			// Обходим все значения Y
			for (i = 1; i <= fabs(l_intDY); i++)
			{
				int y = in_intY1 - i;
				ImpactType l_eResult = checkColor(in_pPixelBuffer, x, y, true);
				if (l_eResult != IMPACT_NONE)
				{
					return l_eResult;
				}
				l_fError += l_fSlope;
				if (l_fError <= -0.5)
				{
					x++;
					l_fError += 1;
				}
			}
		}
	}

	return IMPACT_NONE;
}