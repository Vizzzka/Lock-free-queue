#include <gtest/gtest.h>
#include "LockFreeQueue/LockFreeQueue.h"
#include "BlockingQueue//BlockingQueue.h"
#include "QueueInterface/QueueInterface.h"


template <typename T> class ConcurrentQueueTest: public ::testing::Test { };
TYPED_TEST_CASE_P(ConcurrentQueueTest);

// placeholder for test bodies

TYPED_TEST_P(ConcurrentQueueTest, ShouldAdd1) {
    TypeParam container;
    container.push(1);
    ASSERT_EQ(container.pop(), 1);
}

REGISTER_TYPED_TEST_CASE_P(ConcurrentQueueTest, ShouldAdd1);


typedef ::testing::Types <BlockingQueue<int>>  QueueTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PlaceholderName, ConcurrentQueueTest, QueueTypes);



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}