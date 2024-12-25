#include <iostream>
#include <cmath>
#include "integral.h"

extern "C" {

//методом прямоугольников
float integralFirst(float A, float B, float e) { 
    float integral = 0;  //сумма площадей прямоугольников
    float dx = (B - A) / e; //dx - ширина каждого прямоугольника. //sin(x) - высота
    for (float x = A; x < B; x += dx) { // цикл по всем прямоугольникам.
        integral += std::sin(x) * dx; // добавляем площадь текущего прямоугольника к общей сумм
    }
    return integral;  // Возвращаем интеграл
}


//методом трапеций
float integralSecond(float A, float B, float e) { 
float integral = 0;  
    float dx = (B - A) / e;
    for (float x = A; x < B; x += dx) {
        integral += (std::sin(x) + std::sin(x + dx)) * dx / 2; //Площадь трапеции вычисляется как среднее значение высот на краях трапеции, умноженное на ширину.
    }
    return integral;  // Возвращаем интеграл
}
}