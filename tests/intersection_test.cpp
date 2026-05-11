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
    Shape sh1 = line1, sh2 = line2;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_TRUE(res.has_value());
    auto d = points_dist(res.value(), Point2D(3, 3));
    EXPECT_TRUE(std::abs(d) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, LinesNo_Vertical) {
    Shape sh1 = line1, sh2 = line4;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_TRUE(res.has_value());
    auto d = points_dist(res.value(), Point2D(7, 7));
    EXPECT_TRUE(std::abs(d) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, LinesNo) {
    Shape sh1 = line1, sh2 = line3;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, LinesNo_Horizontal) {
    Shape sh1 = line1, sh2 = line5;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, LineCircleNo) {
    Shape sh1 = line1, sh2 = circle1;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, LineCircleYes) {
    Shape sh1 = line1, sh2 = circle3;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_TRUE(res.has_value());
    auto d1 = points_dist(res.value(), Point2D(7, 7));
    auto d2 = points_dist(res.value(), Point2D(8, 8));
    EXPECT_TRUE(std::abs(d1) <= std::numeric_limits<double>::epsilon() ||
                std::abs(d2) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, CirclesYes) {
    Shape sh1 = circle1, sh2 = circle2;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_TRUE(res.has_value());
    auto d = points_dist(res.value(), Point2D(8, 2));
    EXPECT_TRUE(std::abs(d) <= std::numeric_limits<double>::epsilon());
}

TEST(IntersectionsTest, CirclesNo) {
    Shape sh1 = circle1, sh2 = circle3;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, CirclesYes_2Points) {
    Shape sh1 = circle2, sh2 = circle5;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_TRUE(res.has_value());
    auto d1 = points_dist(res.value(), Point2D(10, 4));
    auto d2 = points_dist(res.value(), Point2D(11.6, 3.2));
    EXPECT_TRUE(std::abs(d1) <= 0.5 || std::abs(d2) <= 0.5);
}

TEST(IntersectionsTest, CirclesNo_Inside) {
    Shape sh1 = circle2, sh2 = circle4;
    auto res = intersections::GetIntersectPoint(sh1, sh2);
    EXPECT_FALSE(res.has_value());
}

TEST(IntersectionsTest, Shapes_Excetion) {
    Shape sh1 = triangle, sh2 = rectancle;
    EXPECT_THROW(intersections::GetIntersectPoint(sh1, sh2), std::logic_error);
}