#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

TEST(sstsRemove, Scheduled)
{
    ssts::task_scheduler s(4);

	s.in("task_id_1"s, 2s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 2s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 2s, []{std::cout << "Hello C!" << std::endl;});

    EXPECT_TRUE(s.is_scheduled("task_id_1"s));
    EXPECT_TRUE(s.is_scheduled("task_id_2"s));
    EXPECT_TRUE(s.is_scheduled("task_id_3"s));
}

TEST(sstsRemove, RemovedAfterRun)
{
    ssts::task_scheduler s(4);

	s.in("task_id_1"s, 1s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 1s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 1s, []{std::cout << "Hello C!" << std::endl;});

    std::this_thread::sleep_for(2s);

    EXPECT_FALSE(s.is_scheduled("task_id_1"s));
    EXPECT_FALSE(s.is_scheduled("task_id_2"s));
    EXPECT_FALSE(s.is_scheduled("task_id_3"s));
}

TEST(sstsRemove, RemoveAdd)
{
    ssts::task_scheduler s(4);

	s.in("task_id_1"s, 1s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 1s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 1s, []{std::cout << "Hello C!" << std::endl;});

	s.remove_task("task_id_1"s);
	s.remove_task("task_id_2"s);
	s.remove_task("task_id_3"s);

    EXPECT_TRUE(s.is_scheduled("task_id_1"s));
    EXPECT_TRUE(s.is_scheduled("task_id_2"s));
    EXPECT_TRUE(s.is_scheduled("task_id_3"s));

    std::this_thread::sleep_for(2s);

    EXPECT_FALSE(s.is_scheduled("task_id_1"s));
    EXPECT_FALSE(s.is_scheduled("task_id_2"s));
    EXPECT_FALSE(s.is_scheduled("task_id_3"s));
}