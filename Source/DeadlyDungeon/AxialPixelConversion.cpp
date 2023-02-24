// Fill out your copyright notice in the Description page of Project Settings.


#include "AxialPixelConversion.h"

FVector AxialPixelConversion::axialToPixel(int r, int q)
{
    constexpr int size{ 104 };

    int x = size * (3. / 2 * q);
    int y = size * (sqrt(3) / 2 * q + sqrt(3) * r);
    return FVector(x, y, 0);
}



AxialPixelConversion::AxialPixelConversion()
{


}

AxialPixelConversion::~AxialPixelConversion()
{
}
