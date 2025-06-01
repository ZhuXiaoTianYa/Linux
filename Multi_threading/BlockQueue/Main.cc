#include <stdio.h>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#include "BlockQueue.hpp"
#include "Task.hpp"

// C: 计算
// S: 存储

template <class C, class S>
class BlockQueues
{
public:
    BlockQueue<C> *c_bq;
    BlockQueue<S> *s_bq;
};

void *productor(void *bqs)
{
    BlockQueue<CalTask> *bq = (static_cast<BlockQueues<CalTask, SaveTask> *>(bqs))->c_bq;
    while (true)
    {
        int x = rand() % 100 + 1;
        int y = rand() % 10;
        int operCode = rand() % oper.size();
        CalTask t(x, y, oper[operCode], mymath);
        bq->push(t);
        std::cout << "protuctor thread, 产生计算任务: " << t.toTaskString() << std::endl;
    }
    return nullptr;
}

void *consumer(void *bqs)
{
    BlockQueue<CalTask> *cal_bq = (static_cast<BlockQueues<CalTask, SaveTask> *>(bqs))->c_bq;
    BlockQueue<SaveTask> *save_bq = (static_cast<BlockQueues<CalTask, SaveTask> *>(bqs))->s_bq;
    while (true)
    {
        // 消费活动
        CalTask t;
        cal_bq->pop(&t);
        std::string result = t();
        std::cout << "cal thread. 完成计算任务: " << result << " ...done" << std::endl;
        SaveTask save(result, Save);
        save_bq->push(save);
        std::cout << "cal thread. 推送储存任务完成..." << std::endl;
    }
    return nullptr;
}

void *saver(void *bqs)
{
    BlockQueue<SaveTask> *save_bq = (static_cast<BlockQueues<CalTask, SaveTask> *>(bqs))->s_bq;
    while (true)
    {
        SaveTask t;
        save_bq->pop(&t);
        t();
        std::cout << "save thread. 保存任务完成..." << std::endl;
    }
    return nullptr;
}

int main()
{
    srand(time(nullptr));
    BlockQueues<CalTask, SaveTask> bqs;
    bqs.c_bq = new BlockQueue<CalTask>();
    bqs.s_bq = new BlockQueue<SaveTask>();

    pthread_t c[2], p[3], s;

    pthread_create(p, nullptr, productor, &bqs);
    pthread_create(p + 1, nullptr, productor, &bqs);
    pthread_create(p + 2, nullptr, productor, &bqs);
    pthread_create(c, nullptr, consumer, &bqs);
    pthread_create(c + 1, nullptr, consumer, &bqs);

    pthread_create(&s, nullptr, saver, &bqs);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    pthread_join(p[2], nullptr);
    pthread_join(s, nullptr);

    delete bqs.c_bq;
    delete bqs.s_bq;
    return 0;
}