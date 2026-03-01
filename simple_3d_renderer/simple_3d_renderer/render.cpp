#include "projection.h"
#include <windows.h>
#include <iostream>

void render(std::vector<float> translation_vector) {
    {
        system("cls"); //clear screen

        Cube3D c;

        for (auto& v : c.points) {
            v.applyVector(translation_vector);
        }

        Cube2D c2D = projectCube3D(c, 10);

        //print points
        for (auto& p : c2D.points) {
            std::cout << "\x1b[33m(" << p.x << "," << p.y << ")" << "\n";
        }

        //find max x and y
        float max_x = c2D.points[0].x;
        float max_y = c2D.points[0].y;

        for (auto& p : c2D.points) {
            if (p.x > max_x) {
                max_x = p.x;
            }

            if (p.y > max_y) {
                max_y = p.y;
            }
        }

        //find min x and y
        float min_x = c2D.points[0].x;
        float min_y = c2D.points[0].y;

        for (auto& p : c2D.points) {
            if (p.x < min_x) {
                min_x = p.x;
            }

            if (p.y < min_y) {
                min_y = p.y;
            }
        }

        min_x -= 5;
        min_y -= 5;
        max_y += 5;
        max_x += 5;

        //print cube
        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                bool found = false;

                for (auto& p : c2D.points) {
                    if (round(p.x) == x && round(p.y) == y) {
                    //if ((round(p.x) == x && round(p.y) == y) || isPointInPolygon(c2D.points[c2D.faces[0]], p) {
                        found = true;
                        break;
                    }
                }

                if (found) {
                    std::cout << "\x1b[31m#";
                }
                else {
                    std::cout << "  ";
                }
            }
            std::cout << "\n";
        }

    }
}

int main()
{
    bool loop = true;

    std::vector<float> translation_vector = { 0,0,0 };

    render(translation_vector);

    while (loop)
    {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            loop = false;
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            translation_vector[0] += 0.01;
            render(translation_vector);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            translation_vector[0] -= 0.01;
            render(translation_vector);
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            translation_vector[1] += 0.01;
            render(translation_vector);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            translation_vector[1] -= 0.01;
            render(translation_vector);
        }
    }

    return 1;
}
