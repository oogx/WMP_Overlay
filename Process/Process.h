#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <ShlObj.h>
#include <thread>
#include <d3d9.h>
#include <dwmapi.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    bool operator!=(const Vec3& vector) const { return x != vector.x || y != vector.y || z != vector.z; }
    float length() const { return sqrt((x * x) + (y * y) + (z * z)); }
    float distance(const Vec3& Vec) const { return (*this - Vec).length(); }
    Vec3 normalized() const { return Vec3(x / length(), y / length(), z / length()); }
};
namespace Process {
    extern int Monitor_X;
    extern int Monitor_Y;
    extern HWND Hwnd;
    extern Vec3 Position;
    extern Vec3 Size;
    bool Setup(bool Enable);
    HANDLE Start(HANDLE Parent, const char* Path);
}