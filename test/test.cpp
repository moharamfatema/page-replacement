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
    std::vector<std::vector<  int>> correctTrace = {
        {5},
        {5, 12},
        {5, 12},
        {5, 12, 2},
        {4, 12, 2},
        {4, 12, 2},
        {4, 5, 2}};
        paging.go();
    EXPECT_THAT(paging.getTrace(), ContainerEq(correctTrace));
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
    EXPECT_EQ(paging.getNoOfPageFaults(),2);
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
    EXPECT_THAT(paging2.go(), ContainerEq(correctTrace));
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
    
    EXPECT_EQ(paging2.getNoOfPageFaults(),6);
}

TEST(TestPageReplacement,testOptimal)
{
    PageReplacement paging(3, "OPTIMAL", {2,3,2,1,5,2,4,5,3,2,5,2});
    std::vector<std::vector<  int>> correctTrace = {
        {2},
        {2, 3},
        {2, 3},
        {2, 3, 1},
        {2, 3, 5},
        {2, 3, 5},
        {4, 3, 5},
        {4, 3, 5},
        {4, 3, 5},
        {2,3,5},
        {2,3,5},
        {2,3,5}
    };
    EXPECT_THAT(paging.go(), ContainerEq(correctTrace));
    EXPECT_THAT(
        paging.getPageFaults(),
        ElementsAre(
            false,
            false,
            false,
            false,
            true,
            false,
            true,
            false,
            false,
            true,
            false,
            false));
    EXPECT_EQ(paging.getNoOfPageFaults(),3);
}

TEST(TestPageReplacement,testPredict)
{
    PageReplacement paging(3, "OPTIMAL", {2,3,2,1,5,2,4,5,3,2,5,2});
    std::vector<  int> mockFrames = {2,3,1};
    auto it = paging.getSequence().begin() + 4;
    paging.setFrames(mockFrames);
    EXPECT_EQ(*paging.predict(it),1);
    mockFrames[2] = 5;
    paging.setFrames(mockFrames);
    EXPECT_EQ(*paging.predict(it+2),2);
    mockFrames[0] = 4;
    paging.setFrames(mockFrames);
    EXPECT_EQ(*paging.predict(it+3),4);
}

TEST(TestPageReplacement,testClock)
{
    PageReplacement paging(3, "CLOCK", {2,3,2,1,5,2,4,5,3,2,5,2});
    std::vector<std::vector<  int>> correctTrace = {
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
        {3, 2, 5},
        {3, 2, 5},
    };
    
    EXPECT_THAT(paging.go(),ContainerEq(correctTrace));
    EXPECT_THAT(
        paging.getPageFaults(),
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
            false));
    EXPECT_EQ(5,paging.getNoOfPageFaults());
}

TEST(TestPageReplacement,testLestRecentlyUsed)
{
    PageReplacement paging(3, "LRU", {2,3,2,1,5,2,4,5,3,2,5,2});
    std::vector<  int> mockFrames = {2,3,1};

    int index = 4;
    paging.setFrames(mockFrames);
    EXPECT_EQ(3,*paging.leastRecentlyUsed(index));
    mockFrames[1] = 5;
    index+=2;
    paging.setFrames(mockFrames);
    EXPECT_EQ(1,*paging.leastRecentlyUsed(index));
    mockFrames[2] = 4;
    index +=2;
    paging.setFrames(mockFrames);
    EXPECT_EQ(2,*paging.leastRecentlyUsed(index));
    mockFrames[0] = 3;
    index++;
    paging.setFrames(mockFrames);
    EXPECT_EQ(4,*paging.leastRecentlyUsed(index));
}

TEST(TestPageReplacement,testLRU)
{
    PageReplacement paging(3, "LRU", {2,3,2,1,5,2,4,5,3,2,5,2});
    std::vector<std::vector<  int>> correctTrace = {
        {2},
        {2, 3},
        {2, 3},
        {2, 3, 1},
        {2, 5, 1},
        {2, 5, 1},
        {2, 5, 4},
        {2, 5, 4},
        {3, 5, 4},
        {3, 5, 2},
        {3, 5, 2},
        {3, 5, 2},
    };
    
    EXPECT_THAT(paging.go(),ContainerEq(correctTrace));
    EXPECT_THAT(
        paging.getPageFaults(),
        ElementsAre(
            false,
            false,
            false,
            false,
            true,
            false,
            true,
            false,
            true,
            true,
            false,
            false));
    EXPECT_EQ(4,paging.getNoOfPageFaults());
}