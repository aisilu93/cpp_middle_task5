#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <print>
#include <ranges>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");

    /*
     * Используйте ranges чтобы оставить только фигуры,
     * поддерживающие возможность находить пересечения между собой
     *
     * Затем примените монадический интерфейс для обработки результатов:
     *     - Пересечение найдено в точке A между фигурами B и C
     *     - Фигуры B и C не пересекаются
     */

    auto f = [](const auto &sh) {
        if (holds_alternative<Line>(sh) || holds_alternative<Circle>(sh))
            return true;
        return false;
    };

    if (!f(shape))
        return;

    auto filtered = others | views::filter(f);

    for (const auto &s : filtered) {
        intersections::GetIntersectPoint(shape, s).and_then([&](const auto &point) {
            std::print("Пересечение найдено в точке {} между фигурами ", point);
            std::visit([&](const auto &x) { std::print("{} и ", x); }, std::move(shape));
            std::visit([&](const auto &x) { std::println("{}", x); }, std::move(s));
            return std::optional<Point2D>{};
        });
    }
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    /*
     * Используйте ranges чтобы выбрать любые 5 фигур из списка.
     * Затем найдите расстояния от заданной точки до всех выбранных фигур.
     * Выведите результат в формате "Расстояние от точки P до фигуры S равно D"
     */

    auto filtered = shapes | views::take(5);
    rng::for_each(filtered, [&](const auto &shape) {
        auto res = std::visit(queries::PointToShapeDistanceVisitor(p), shape);
        std::print("Расстояние от точки {} до фигуры ", p);
        std::visit([&](const auto &x) { std::println("{} равно {} ", x, res); }, std::move(shape));
    });
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Найти все пересечения между фигурами используя метод Bounding Box
     *     - Найти самую высокую фигуру (чья высота наибольшая)expected
     *     - Вывести расстояние между любыми двумя фигурами, которые поддерживают данную функциональность
     */

    bool distance_found = false;
    for (auto i = 0; i < shapes.size(); i++)
        for (auto j = i + 1; j < shapes.size(); j++) {
            if (queries::BoundingBoxesOverlap(shapes[i], shapes[j])) {
                std::visit([&](const auto &x) { std::print("Фигура {} пересекается с ", x); }, shapes[i]);
                std::visit([&](const auto &x) { std::println("фигурой {}", x); }, shapes[j]);
            }
            if (!distance_found)
                std::visit(queries::ShapeToShapeDistanceVisitor{}, shapes[i], shapes[j])
                    .and_then([&](const auto &dist) {
                        std::visit([&](const auto &x) { std::print("Расстояние между фигурами {} и ", x); }, shapes[i]);
                        std::visit([&](const auto &x) { std::print("{} равно ", x); }, shapes[j]);
                        std::println("{}", dist);
                        distance_found = true;
                        return std::optional<double>{};
                    });
        }

    utils::FindHighestShape(shapes).transform([&](const auto &pos) {
        std::visit([&](const auto &x) { std::println("Самая высокая фигура: {}", x); }, shapes[pos]);
        return true;
    });
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Вывести 3 любые фигуры, которые находятся выше 50.0
     *     - Вывести фигуры с наименьшей и с наибольшей высотами
     */

    shapes | views::filter([](const auto &shape) {
        if (queries::GetHeight(shape) > 50.f)
            return true;
        return false;
    }) | views::take(3) |
        views::transform([](const auto &shape) {
            std::visit([&](const auto &x) { std::println("Фигура находится выше 50.0: {}", x); }, shape);
            return true;
        });

    auto comp = [](const auto &lhs, const auto &rhs) { return queries::GetHeight(lhs) < queries::GetHeight(rhs); };
    rng::max_element(shapes, comp)->visit([&](const auto &x) { std::println("Фигура с наибольшей высотой: {}", x); });
    rng::min_element(shapes, comp)->visit([&](const auto &x) { std::println("Фигура с наименьшей высотой: {}", x); });
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                                   "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту
    for (const auto &[index, shape] : views::enumerate(shapes))
        shape.visit([&](const auto &x) { std::println("{}: {}", index, x); });
    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes, "plot1.png");

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;

    auto get_points = [&](const auto &a) {
        auto verts = a.Vertices();
        rng::copy(verts, std::back_inserter(points));
    };

    rng::for_each(shapes, [&](const auto &sh) { sh.visit(get_points); });

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    auto graham_points = convex_hull::GrahamScan(points);
    if (graham_points) {
        shapes.emplace_back(Polygon(graham_points.value()));
    } else {
        std::println("Graham scan error {}", graham_points.error());
    }

    visualization::Draw(shapes, "plot2.png");

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //

        auto triangles = triangulation::DelaunayTriangulation(points);
        if (triangles) {
            visualization::Draw(triangles.value());
        } else {
            std::println("DelaunayTriangulation error {}", triangles.error());
        }
    }
    return 0;
}