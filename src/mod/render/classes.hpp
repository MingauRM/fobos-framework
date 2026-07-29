#include <algorithm>

class Window {
    public:
        Window()
        {

        }

    private:
        

};

class Vec3 {
    public:
        double X;
        double Y;
        double Z;

        Vec3(double x, double y, double z)
        {
            X = x;
            Y = y;
            Z = z;
        }

    private:

};

class Color {
    public:
        double R;
        double G;
        double B;
        double A;

    Color(double r, double g, double b, double a)
    {
        R = std::clamp(r, 0.0, 1.0);
        G = std::clamp(g, 0.0, 1.0);
        B = std::clamp(b, 0.0, 1.0);
        A = std::clamp(a, 0.0, 1.0);
    }

    private:

};

class Vertex {
    public:
        Vertex(Vec3 position, Color color)
        {
            
        }
    
    private:

};