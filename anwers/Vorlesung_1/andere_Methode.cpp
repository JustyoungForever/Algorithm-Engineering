#include <iostream>
#include <cmath>

const int ITERATIONS = 10;  // 可以增加迭代次数以获得更高的精度

int main() {
    double innerPolygonHalfPerimeter = 3.0; 
    double outerPolygonHalfPerimeter = 3.0; 

    double innerSideLength = 1.0;  
    double outerSideLength = tan(M_PI / 6);  

    for (int i = 0; i < ITERATIONS; ++i) {
        
        innerSideLength = sqrt(2 - sqrt(4 - innerSideLength * innerSideLength));
        
        
        outerSideLength = outerSideLength / (sqrt(2 + sqrt(4 - outerSideLength * outerSideLength)));

        innerPolygonHalfPerimeter *= 2;
        outerPolygonHalfPerimeter *= 2;

        
        double lowerBound = innerPolygonHalfPerimeter * innerSideLength;
        double upperBound = outerPolygonHalfPerimeter * outerSideLength;

        std::cout << "After " << (6 * pow(2, i)) << " sides: " << std::endl;
        std::cout << "Lower bound: " << lowerBound << std::endl;
        std::cout << "Upper bound: " << upperBound << std::endl << std::endl;
    }

    return 0;
}
