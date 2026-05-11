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
    Shape sh = line6;
    auto res = queries::DistanceToPoint(sh, point1);
    EXPECT_NEAR(res, 2.2360679, 1e-6);
}

TEST(QueriesTest, Distance_PointToCircle) {
    Shape sh = circle6;
    auto res = queries::DistanceToPoint(sh, point1);
    EXPECT_NEAR(res, 1.2360679, 1e-6);
}

TEST(QueriesTest, Distance_PointToTriangle) {
    Shape sh = triangle2;
    auto res = queries::DistanceToPoint(sh, point1);
    EXPECT_NEAR(res, 0.624695, 1e-6);
}

TEST(QueriesTest, Distance_PointToRectangle) {
    Shape sh = rectancle2;
    auto res = queries::DistanceToPoint(sh, point1);
    EXPECT_NEAR(res, 2.0, 1e-6);
}

TEST(QueriesTest, Distance_CircleToTriangle) {
    Shape sh1 = circle6, sh2 = triangle2;
    auto res = queries::DistanceBetweenShapes(sh1, sh2);
    EXPECT_FALSE(res.has_value());
}

TEST(QueriesTest, Distance_CircleToLine) {
    Shape sh1 = circle6, sh2 = line6;
    auto res = queries::DistanceBetweenShapes(sh1, sh2);
    EXPECT_FALSE(res.has_value());
}

TEST(QueriesTest, Distance_CircleToItsOwn) {
    Shape sh = circle6;
    auto res = queries::DistanceBetweenShapes(sh, sh);
    EXPECT_TRUE(res.has_value());
    EXPECT_NEAR(res.value(), 0.0, 1e-6);
}

TEST(QueriesTest, Distance_CircleToCircle) {
    Shape sh1 = circle6, sh2 = circle7;
    auto res = queries::DistanceBetweenShapes(sh1, sh2);
    EXPECT_TRUE(res.has_value());
    EXPECT_NEAR(res.value(), 1.0, 1e-6);
}

TEST(QueriesTest, Distance_LineToLine) {
    Shape sh1 = line6, sh2 = line7;
    auto res = queries::DistanceBetweenShapes(sh1, sh2);
    EXPECT_TRUE(res.has_value());
    EXPECT_NEAR(res.value(), 3.0, 1e-6);
}

TEST(QueriesTest, GetBoundBox_Line) {
    Shape shape = line6;
    auto box = queries::GetBoundBox(shape);
    EXPECT_DOUBLE_EQ(box.min_x, 3);
    EXPECT_DOUBLE_EQ(box.min_y, 5);
    EXPECT_DOUBLE_EQ(box.max_x, 4);
    EXPECT_DOUBLE_EQ(box.max_y, 5);
}

TEST(QueriesTest, GetBoundBox_Circle) {
    Shape shape = circle6;
    auto box = queries::GetBoundBox(shape);
    EXPECT_DOUBLE_EQ(box.min_x, 6);
    EXPECT_DOUBLE_EQ(box.min_y, 1);
    EXPECT_DOUBLE_EQ(box.max_x, 8);
    EXPECT_DOUBLE_EQ(box.max_y, 3);
}

TEST(QueriesTest, GetHeight_Line) {
    Shape shape = line6;
    auto height = queries::GetHeight(shape);
    EXPECT_DOUBLE_EQ(height, 5);
}

TEST(QueriesTest, GetHeight_Circle) {
    Shape shape = circle6;
    auto height = queries::GetHeight(shape);
    EXPECT_DOUBLE_EQ(height, 3);
}
