// Рассчет интеграла функции sin(x) на отрезке [A, B] с шагом e
#pragma once
extern "C" {
float integralFirst(float A, float B, float e); //Подсчет интеграла методом прямоугольников.
float integralSecond(float A, float B, float e); //Подсчет интеграла методом трапеций.
}