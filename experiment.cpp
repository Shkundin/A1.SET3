
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <cstdint>
#include <cmath>

template <class Inside>
double monte_carlo(Inside inside,
    double x0, double x1,
    double y0, double y1,
    long long samples,
    std::uint64_t seed = 1234567)
{
    if (samples <= 0) return 0.0;
    if (x0 > x1) std::swap(x0, x1);
    if (y0 > y1) std::swap(y0, y1);

    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> X(x0, x1);
    std::uniform_real_distribution<double> Y(y0, y1);

    long long hit = 0;
    for (long long s = 0; s < samples; ++s)
    {
        double x = X(rng);
        double y = Y(rng);
        if (inside(x, y)) ++hit;
    }
    const double area_rect = (x1 - x0) * (y1 - y0);
    return area_rect * (double)hit / (double)samples;
}

struct Circle { double x, y, r; };

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);


    const double PI = std::acos(-1.0);


    Circle C[3];
    C[0] = { 1.0, 1.0, 1.0 };
    C[1] = { 1.5, 2.0, std::sqrt(5.0) / 2.0 };
    C[2] = { 2.0, 1.5, std::sqrt(5.0) / 2.0 };

    double r2[3] = {
        C[0].r * C[0].r,
        C[1].r * C[1].r,
        C[2].r * C[2].r
    };


    double xmin = std::max({ C[0].x - C[0].r,
                             C[1].x - C[1].r,
                             C[2].x - C[2].r });
    double xmax = std::min({ C[0].x + C[0].r,
                             C[1].x + C[1].r,
                             C[2].x + C[2].r });
    double ymin = std::max({ C[0].y - C[0].r,
                             C[1].y - C[1].r,
                             C[2].y - C[2].r });
    double ymax = std::min({ C[0].y + C[0].r,
                             C[1].y + C[1].r,
                             C[2].y + C[2].r });


    double wx0 = 0.0, wx1 = 3.0;
    double wy0 = 0.0, wy1 = 3.0;


    const double S_exact = 0.25 * PI + 1.25 * std::asin(0.8) - 1.0;

    auto inside_all = [&](double x, double y) -> bool {
        double dx0 = x - C[0].x; double dy0 = y - C[0].y;
        double dx1 = x - C[1].x; double dy1 = y - C[1].y;
        double dx2 = x - C[2].x; double dy2 = y - C[2].y;
        return (dx0 * dx0 + dy0 * dy0 <= r2[0]) &&
            (dx1 * dx1 + dy1 * dy1 <= r2[1]) &&
            (dx2 * dx2 + dy2 * dy2 <= r2[2]);
        };

    std::uint64_t base_seed = 1234567;

    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(10);


    std::cout << "N,rect_type,area_est,rel_error\n";


    for (long long N = 100; N <= 100000; N += 500)
    {

        double area_wide = monte_carlo(inside_all,
            wx0, wx1, wy0, wy1,
            N, base_seed + N * 2);
        double err_wide = std::fabs(area_wide - S_exact) / S_exact;
        std::cout << N << ",wide," << area_wide << "," << err_wide << "\n";


        double area_narrow = monte_carlo(inside_all,
            xmin, xmax, ymin, ymax,
            N, base_seed + N * 2 + 1);
        double err_narrow = std::fabs(area_narrow - S_exact) / S_exact;
        std::cout << N << ",narrow," << area_narrow << "," << err_narrow << "\n";
    }

    return 0;
}