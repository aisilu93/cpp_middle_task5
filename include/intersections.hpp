#pragma once
#include "geometry.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <optional>
#include <ranges>

namespace geometry::intersections {

/*
 * Класс для поиска пересечений между двумя фигурами
 *
 * Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
 *    - Line   & Line
 *    - Line   & Circle
 *    - Circle & Circle
 *
 * Для всех остальных требуется выбросить исключение std::logic_error
 */
class IntersectionVisitor {
public:
    std::optional<Point2D> operator()(const Shape &shape1, const Shape &shape2) const {
        throw std::logic_error("Unsupported compination of shapes.");
        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Line &line1, const Line &line2) const {
        auto [x1, y1] = line1.start;
        auto [x2, y2] = line1.end;
        auto [x3, y3] = line2.start;
        auto [x4, y4] = line2.end;

        auto d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (std::abs(d) <= std::numeric_limits<double>::epsilon())
            return std::nullopt;

        auto x = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
        x /= d;

        auto y = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
        y /= d;

        if (x >= std::min(x1, x2) && x <= std::max(x1, x2) && x >= std::min(x3, x4) && x <= std::max(x3, x4) &&
            y >= std::min(y1, y2) && y <= std::max(y1, y2) && y >= std::min(y3, y4) && y <= std::max(y3, y4))

            return Point2D(x, y);

        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Line &line, const Circle &circle) const {
        std::optional<Point2D> result;
        const auto &verts = circle.Vertices();
        if (std::ranges::any_of(std::views::iota(size_t{0}, verts.size()), [&](auto i) {
                auto next_i = i == verts.size() - 1 ? 0 : i + 1;
                Line l(verts[i], verts[next_i]);
                result = operator()(line, l);
                return result.has_value();
            }))
            return result;
        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Circle &circle1, const Circle &circle2) const {
        auto [x1, y1] = circle1.Center();
        auto [x2, y2] = circle2.Center();
        auto d = std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

        // круги на расстоянии друг от друга или один внутри другого - пересечений нет
        if ((d > (circle1.radius + circle2.radius)) || (d < std::abs(circle1.radius - circle2.radius)))
            return std::nullopt;

        // круги совпадают, в качестве точки пересечения подойдет любая, лежащая на любой окружности
        if (std::abs(d) <= std::numeric_limits<double>::epsilon() &&
            std::abs(circle1.radius - circle2.radius) <= std::numeric_limits<double>::epsilon())
            return circle1.Vertices()[0];

        auto a = circle1.radius * circle1.radius - circle2.radius * circle2.radius + d * d;
        a /= 2 * d;

        auto x0 = x1 + a / d * (x2 - x1);
        auto y0 = y1 + a / d * (y2 - y1);
        auto h = std::sqrt(circle1.radius * circle1.radius - a * a);

        Point2D result;
        result.x = x0 - h * (y2 - y1) / d;
        result.y = y0 + h * (x2 - x1) / d;

        /*Point2D result2;
        result2.x = x0 + h * (y2 - y1) / d;
        result2.y = y0 - h * (x2 - x1) / d;*/

        return result;
    }
};

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    return std::visit(IntersectionVisitor{}, std::move(shape1), std::move(shape2));
}

}  // namespace geometry::intersections