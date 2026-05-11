#include "geometry.hpp"
#include "queries.hpp"
#include <gtest/gtest.h>

using namespace geometry;

Point2D point1{5, 3};

Line line6({3, 5}, {4, 5});
Line line7({7, 4}, {7, 9});
Circle circle6({7, 2}, 1);
Circle circle7({10, 2}, 1);
Triangle triangle2({1, 1}, {1, 7}, {5, 2});
Rectangle rectancle2({1, 1}, 2, 3);

TEST(QueriesTest, Distance_PointToLine) {
    auto res = queries::DistanceToPoint(line6, point1);
    EXPECT_NEAR(res, 2.2360679, 1e-6);
}

TEST(QueriesTest, Distance_PointToCircle) {
    auto res = queries::DistanceToPoint(circle6, point1);
    EXPECT_NEAR(res, 1.2360679, 1e-6);
}

TEST(QueriesTest, Distance_PointToTriangle) {
    auto res = queries::DistanceToPoint(triangle2, point1);
    EXPECT_NEAR(res, 0.624695, 1e-6);
}

TEST(QueriesTest, Distance_PointToRectangle) {
    auto res = queries::DistanceToPoint(rectancle2, point1);
    EXPECT_NEAR(res, 2.0, 1e-6);
}

TEST(QueriesTest, Distance_CircleToTriangle) {
    auto res = queries::DistanceBetweenShapes(circle6, triangle2);
    EXPECT_FALSE(res.has_value());
}

TEST(QueriesTest, Distance_CircleToLine) {
    auto res = queries::DistanceBetweenShapes(circle6, line6);
    EXPECT_FALSE(res.has_value());
}

TEST(QueriesTest, Distance_CircleToItsOwn) {
    auto res = queries::DistanceBetweenShapes(circle6, circle6);
    EXPECT_TRUE(res.has_value());
    EXPECT_NEAR(res.value(), 0.0, 1e-6);
}

TEST(QueriesTest, Distance_CircleToCircle) {
    auto res = queries::DistanceBetweenShapes(circle6, circle7);
    EXPECT_TRUE(res.has_value());
    EXPECT_NEAR(res.value(), 1.0, 1e-6);
}

TEST(QueriesTest, Distance_LineToLine) {
    auto res = queries::DistanceBetweenShapes(line6, line7);
    EXPECT_TRUE(res.has_value());
    EXPECT_NEAR(res.value(), 3.0, 1e-6);
}
