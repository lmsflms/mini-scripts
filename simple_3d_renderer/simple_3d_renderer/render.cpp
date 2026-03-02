#include "projection.h"
#include <iostream>
#include <Windows.h>
#include <cmath>

VecPoint3D gCubePoints = {
        { 20, 100, 100},
        { 100, 100, 100 },
        { 100, 100, 20 },
        { 20, 100, 20 },
        { 20, 20, 100 },
        { 100, 20, 100 },
        { 100, 20, 20 },
        { 20, 20, 20 }
};
int θ = 0; //theta is the rotation angle
Cube3D gCube = gCubePoints;
Cube2D gCube2D;

void applyRotationY(int θ) {
    Point3D center = gCube.getCubeCenter();

    float rad_θ = θ * 3.1415926f / 180.0f;

    for (int p = 0; p < gCube.points.size(); p++) {
        float x = gCube.originalPoints[p].x - center.x;
        float z = gCube.originalPoints[p].z - center.z;

        gCube.points[p].x = (x * cos(rad_θ) + z * sin(rad_θ)) + center.x;
        gCube.points[p].z = (-x * sin(rad_θ) + z * cos(rad_θ)) + center.z;
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_RIGHT) {

            θ += 1;
            applyRotationY(θ);

            InvalidateRect(hWnd, NULL, TRUE); //calls WM_PAINT
        }
        if (wParam == VK_LEFT) {

            θ -= 1;
            applyRotationY(θ);

            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (wParam == VK_UP) {
            for (auto& p : gCube.points) {
                p.y += 1;
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (wParam == VK_DOWN) {
            for (auto& p : gCube.points) {
                p.y -= 1;
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        //Brush : background color, Pen : outline color
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));
        HPEN hPen = CreatePen(PS_DASH, 3, RGB(255, 0, 0));

        //Apply style
        SelectObject(hdc, hBrush);
        SelectObject(hdc, hPen);

        //================ 

        Cube2D gCube2D = projectCube3D(gCube, 20);

        //for each 6 faces
        for (int f = 0; f < 6; f++) {
            const size_t nPoints = 4;
            std::vector<POINT> faceCorners = {};

            for (int i = 0; i < nPoints; i++) {
                faceCorners.push_back(POINT((long)gCube2D.points[gCube2D.faces[f][i]].x, (long)gCube2D.points[gCube2D.faces[f][i]].y));
            }
            Polygon(hdc, faceCorners.data(), nPoints); //left, top, right, bottom

        }

        EndPaint(hWnd, &ps);

        DeleteObject(hBrush);
        DeleteObject(hPen);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance,
    HINSTANCE,
    PWSTR,
    int nCmdShow)
{
    //Register the window class.
    const wchar_t RENDER_WINDOW[] = L"Render Window Class";

    WNDCLASS wc = { };

    //Pointer to window procedure function that manages messages
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = RENDER_WINDOW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    //Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        RENDER_WINDOW,                  // Window class
        L"Render",                      // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 800,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // ==========================================

    // ==========================================

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}