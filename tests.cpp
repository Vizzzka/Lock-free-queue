#include <gtest/gtest.h>
#include "LockFreeQueue//LockFreeQueue.h"
#include "BlockingQueue//BlockingQueue.h"
#include "QueueInterface/QueueInterface.h"
#include <thread>
#include <vector>


namespace {
    int deathPill = -1;
    size_t test_size = 12000;
    size_t maxNumber = 10;
    size_t num_th = std::thread::hardware_concurrency();
    std::vector<int> Nums;
    void getNums() {
        for(size_t i = 0; i < test_size; ++i)
            Nums.push_back (i);
    }

    void addNumbersProducer(
            size_t producersNum,
            size_t remainderr,
            QueueInterface<int> &target
    )
    {
        for(size_t i = remainderr; i < test_size; i += producersNum)
            target.push(Nums[i]);
    }

    void addNumbersConsumer(size_t producersNum,
                            size_t remainderr,
                            QueueInterface<int> &target) {
        int res;
        for(size_t i = remainderr; i < test_size; i += producersNum) {
            while (true) {
                if (i == test_size - 1)
                    std::cout << i << " ";
                if (target.pop(res))
                    break;
            };
        }
    }

}


template <typename T> class ConcurrentQueueTest: public ::testing::Test { };
TYPED_TEST_CASE_P(ConcurrentQueueTest);



TYPED_TEST_P(ConcurrentQueueTest, ShouldAdd) {
    TypeParam container;
    int resi;
    container.push(2);
    container.push(1);
    container.pop();

    ASSERT_EQ(container.pop(), 1);
    ASSERT_EQ(false, container.pop(resi));
    ASSERT_EQ(false, container.pop(resi));
}

TYPED_TEST_P(ConcurrentQueueTest, MPMC) {

    size_t
            producersNum = std::max(
            std::thread::hardware_concurrency() / 2,
            static_cast<unsigned int>(1)
    ),
            consumersNum = std::max(
            std::thread::hardware_concurrency() / 2,
            static_cast<unsigned int>(1)
    );

    TypeParam q;
    getNums();
    std::vector<std::thread>
            producerThreads,
            consumerThreads;

    for(size_t i = 0; i < producersNum; ++i) {
        producerThreads.push_back(
                std::thread(
                        addNumbersProducer,
                        producersNum,
                        i,
                        std::ref(q)
                )
        );
    }

    for(size_t j = 0; j < consumersNum; ++j)
        consumerThreads.push_back(
                std::thread(
                        addNumbersConsumer,
                        consumersNum,
                        j,
                        std::ref(q)
                )
        );


    for(auto &it: producerThreads)
        it.join();

    for(auto &it: consumerThreads)
        it.join();
    q.push(1);
    ASSERT_EQ(q.pop(), 1);

}


REGISTER_TYPED_TEST_CASE_P(ConcurrentQueueTest, ShouldAdd, MPMC);


typedef ::testing::Types <BlockingQueue<int>, LockFreeQueue<int>>  QueueTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PlaceholderName, ConcurrentQueueTest, QueueTypes);



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}