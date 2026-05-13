#include "shape_utils.hpp"
#include <gtest/gtest.h>

using namespace geometry;
/*

std::vector<std::pair<Shape, Shape>> FindAllCollisions(std::span<const Shape> shapes)
std::optional<size_t> FindHighestShape(std::span<const Shape> shapes)
*/
TEST(ShapeUtilsTest, FindAllCollisions_Empty) {
    std::vector<Shape> shapes;
    auto res = utils::FindAllCollisions(shapes);
    EXPECT_TRUE(res.empty());
}

TEST(ShapeUtilsTest, FindAllCollisions_One) {
    std::vector<Shape> shapes{Line{{1, 1}, {5, 5}}, Circle{{3, 3}, 1}, Circle{{20, 20}, 1}};
    auto res = utils::FindAllCollisions(shapes);
    EXPECT_EQ(res.size(), 1);
}

TEST(ShapeUtilsTest, FindAllCollisions_Many) {
    std::vector<Shape> shapes{Circle{{0, 0}, 5}, Circle{{2, 2}, 3}, Rectangle{{1, 1}, 2, 2}, Circle{{100, 100}, 1}};
    auto res = utils::FindAllCollisions(shapes);
    EXPECT_EQ(res.size(), 3);
}

TEST(ShapeUtilsTest, FindHighestShape_Empty) {
    std::vector<Shape> shapes;
    auto res = utils::FindHighestShape(shapes);
    EXPECT_FALSE(res.has_value());
}

TEST(ShapeUtilsTest, FindHighestShape_Line) {
    std::vector<Shape> shapes{Circle{{0, 0}, 1}, Rectangle{{1, 1}, 2, 3}, Line{{0, 0}, {0, 10}}};
    auto res = utils::FindHighestShape(shapes);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), 2);
}

TEST(ShapeUtilsTest, FindHighestShape_Circle) {
    std::vector<Shape> shapes{Triangle{{0, 0}, {1, 1}, {2, 0}}, Circle{{5, 5}, 10}, Rectangle{{0, 0}, 2, 3}};
    auto res = utils::FindHighestShape(shapes);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), 1);
}
