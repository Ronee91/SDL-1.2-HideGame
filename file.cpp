// Файл для работы с файлом карты
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * @brief Разделение строки по подстроке
 * 
 * @param in_sString - строка для разделения
 * @param in_sSplitter - строка разделитель
 * @param in_pResult - вектор с результатом
 */
void split(std::string in_sString, std::string in_sSplitter, std::vector<std::string> &in_pResult)
{
	in_pResult.push_back(in_sString);
	size_t l_stSplitAt;
	size_t l_stSplitterSize = in_sSplitter.size();
	std::string l_sFrag;
	while (true)
	{
		l_sFrag = in_pResult.back();
		l_stSplitAt = l_sFrag.find(in_sSplitter);
		if (l_stSplitAt == std::string::npos)
		{
			break;
		}
		in_pResult.back() = l_sFrag.substr(0, l_stSplitAt);
		in_pResult.push_back(l_sFrag.substr(l_stSplitAt + l_stSplitterSize, l_sFrag.size() - (l_stSplitAt + l_stSplitterSize)));
	}
}
// Точкка координат
struct mapPoint
{
	int x;
	int y;
};

typedef std::vector<mapPoint> PoligonPoints;			// Вектор с точками координат полигона
typedef std::vector<PoligonPoints> PoligonsCoordinates; // Вектор с полигонами
int g_intMaxPoint = 4;									// Максимальная точка в карте, нужна для маштабирования карты под экран

/**
 * @brief Проверка и установка максимальной точки карты
 * 
 * @param in_intCoord 
 */
void checkAndSetMaxPoint(int in_intCoord)
{
	int l_intAbs = abs(in_intCoord);
	if (l_intAbs > g_intMaxPoint)
	{
		g_intMaxPoint = l_intAbs;
	}
}
/**
 * @brief Парсинг текста карты на координаты
 * 
 * @param in_sMap 
 * @return PoligonsCoordinates 
 */
PoligonsCoordinates parseMap(std::string in_sMap)
{
	PoligonsCoordinates l_vPoligonsCoordinates;
	std::vector<std::string> l_vPoligon;
	// Разделяем по квадратной скобке, отделяем полигоны
	split(in_sMap, "[", l_vPoligon);
	for (int i = 0; i < l_vPoligon.size(); i++)
	{
		// Перебираем полигоны
		if (l_vPoligon[i].size() > 0)
		{
			PoligonPoints l_vPoints;
			std::vector<std::string> l_vPoint;
			// Разделяем по круглой скобке, отделяем точки
			split(l_vPoligon[i], "(", l_vPoint);
			for (int j = 0; j < l_vPoint.size(); j++)
			{
				// Перебираем тчки
				if (l_vPoint[j].size() > 0)
				{
					std::vector<std::string> l_vCoord;
					// Разделяем по запятой, отделяем координаты
					split(l_vPoint[j], ",", l_vCoord);
					bool l_bHaveX = false;
					bool l_bHaveY = false;
					mapPoint l_oPoint;
					for (int t = 0; t < l_vCoord.size(); t++)
					{
						if (l_vCoord[t].size() > 0)
						{
							if (l_bHaveX)
							{
								if (l_bHaveY)
								{
									// Третья координата лишняя
									printf("Найдена лишняя координата при загрузке карты из файла! \n");
								}
								else
								{
									// Если координату Х уже добавили эту считаем Y
									l_oPoint.y = std::stof(l_vCoord[t]) * 10;
									l_bHaveY = true;
									checkAndSetMaxPoint(l_oPoint.y);
								}
							}
							else
							{
								// Если координату ещё не добавили считаем её Х
								l_oPoint.x = std::stof(l_vCoord[t]) * 10;
								l_bHaveX = true;
								checkAndSetMaxPoint(l_oPoint.x);
							}
						}
					}
					// Добавляем точку
					l_vPoints.push_back(l_oPoint);
				}
			}
			// ДОбавляем полигон
			l_vPoligonsCoordinates.push_back(l_vPoints);
		}
	}
	return l_vPoligonsCoordinates;
}

/**
 * @brief Чтение из файла
 * 
 * @return PoligonsCoordinates 
 */
PoligonsCoordinates readFile()
{
	// Открываем файл
	std::ifstream l_inFile;
	l_inFile.open("tests/map.txt");
	// Читаем файл
	std::stringstream l_strStream;
	l_strStream << l_inFile.rdbuf();
	std::string l_sMap = l_strStream.str();
	// Закрываем файл
	l_inFile.close();
	return parseMap(l_sMap);
}