#include "geometry.hpp"
#include "intersections.hpp"
#include <gtest/gtest.h>
#include <limits>

using namespace geometry;

double points_dist(const Point2D &a, const Point2D &b) {
    auto [x1, y1] = a;
    auto [x2, y2] = b;

    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

Line line1({1, 1}, {8, 8});
Line line2({1, 7}, {4, 1});
Line line3({3, 6}, {7, 11});
Line line4({7, 4}, {7, 9});
Line line5({3, 5}, {4, 5});

Circle circle1({7, 2}, 1);
Circle circle2({10, 2}, 2);
Circle circle3({8, 7}, 1);
Circle circle4({10, 2}, 1);
Circle circle5({11, 4}, 1);

Triangle triangle({1, 1}, {1, 7}, {5, 2});
Rectangle rectancle({1, 1}, 2, 3);

TEST(IntersectionsTest, LinesYes) {
    auto res = intersections::GetIntersectPoint(line1, line2);
    EXPECT_TRUE(res.has_value());
    auto d = points_dist(res.value(), Point2D(3, 3));
    EXPECT_TRUE(std::abs(d) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, LinesNo_Vertical) {
    auto res = intersections::GetIntersectPoint(line1, line4);
    EXPECT_TRUE(res.has_value());
    auto d = points_dist(res.value(), Point2D(7, 7));
    EXPECT_TRUE(std::abs(d) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, LinesNo) {
    auto res = intersections::GetIntersectPoint(line1, line3);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, LinesNo_Horizontal) {
    auto res = intersections::GetIntersectPoint(line1, line5);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, LineCircleNo) {
    auto res = intersections::GetIntersectPoint(line1, circle1);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, LineCircleYes) {
    auto res = intersections::GetIntersectPoint(line1, circle3);
    EXPECT_TRUE(res.has_value());
    auto d1 = points_dist(res.value(), Point2D(7, 7));
    auto d2 = points_dist(res.value(), Point2D(8, 8));
    EXPECT_TRUE(std::abs(d1) <= std::numeric_limits<double>::epsilon() ||
                std::abs(d2) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, CirclesYes) {
    auto res = intersections::GetIntersectPoint(circle1, circle2);
    EXPECT_TRUE(res.has_value());
    auto d = points_dist(res.value(), Point2D(8, 2));
    EXPECT_TRUE(std::abs(d) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, CirclesNo) {
    auto res = intersections::GetIntersectPoint(circle1, circle3);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, CirclesYes_2Points) {
    auto res = intersections::GetIntersectPoint(circle2, circle5);
    EXPECT_TRUE(res.has_value());
    auto d1 = points_dist(res.value(), Point2D(10, 4));
    auto d2 = points_dist(res.value(), Point2D(11.6, 3.2));
    EXPECT_TRUE(std::abs(d1) <= 0.5 || std::abs(d2) <= 0.5);
}

TEST(IntersectionsTest, CirclesNo_Inside) {
    auto res = intersections::GetIntersectPoint(circle2, circle4);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, Shapes_Excetion) {
    EXPECT_THROW(intersections::GetIntersectPoint(triangle, rectancle), std::logic_error);
}