#include <gtest/gtest.h>
#include "../src/main.cpp"
#include <gmock/gmock.h>

using ::testing::ContainerEq;
using ::testing::ElementsAre;
TEST(TestPageReplacement, testConstructor)
{

    PageReplacement paging(3, "FIFO", {5, 12, 5, 2, 4, 2, 5});
    EXPECT_EQ(paging.getAlgo(), "FIFO");
    EXPECT_EQ(paging.getNoofFrames(), 3);
    EXPECT_EQ(paging.getFrames().size(), 0);
    EXPECT_THAT(paging.getSequence(), ElementsAre(5, 12, 5, 2, 4, 2, 5));
}

TEST(TestPageReplacement, testFIFO)
{
    PageReplacement paging(3, "FIFO", {5, 12, 5, 2, 4, 2, 5});
    std::vector<std::deque<unsigned int>> correctTrace = {
        {5},
        {5, 12},
        {5, 12},
        {5, 12, 2},
        {4, 12, 2},
        {4, 12, 2},
        {4, 5, 2}};
    EXPECT_THAT(paging.fifo(), ContainerEq(correctTrace));
    EXPECT_THAT(
        paging.getPageFaults(),
        ElementsAre(
            false,
            false,
            false,
            false,
            true,
            false,
            true));
    PageReplacement paging2(3, "FIFO", {2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2});
    correctTrace = {
        {2},
        {2, 3},
        {2, 3},
        {2, 3, 1},
        {5, 3, 1},
        {5, 2, 1},
        {5, 2, 4},
        {5, 2, 4},
        {3, 2, 4},
        {3, 2, 4},
        {3, 5, 4},
        {3, 5, 2}};
    EXPECT_THAT(paging2.fifo(), ContainerEq(correctTrace));
    EXPECT_THAT(
        paging2.getPageFaults(),
        ElementsAre(
            false,
            false,
            false,
            false,
            true,
            true,
            true,
            false,
            true,
            false,
            true,
            true));
}