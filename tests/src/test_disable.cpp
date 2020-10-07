#include "gtest/gtest.h"
#include <ssts/task_scheduler.hpp>

TEST(sstsDisable, Enabled)
{
    ssts::task_scheduler s(4);

    s.in("task_id_1"s, 2s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 2s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 2s, []{std::cout << "Hello C!" << std::endl;});

    EXPECT_TRUE(s.is_enabled("task_id_1"s));
    EXPECT_TRUE(s.is_enabled("task_id_2"s));
    EXPECT_TRUE(s.is_enabled("task_id_3"s));
}

TEST(sstsDisable, Disabled)
{
    ssts::task_scheduler s(4);

    s.in("task_id_1"s, 2s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 2s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 2s, []{std::cout << "Hello C!" << std::endl;});

    // Disable tasks before they can run
    s.set_enabled("task_id_1"s, false);
    s.set_enabled("task_id_2"s, false);
    s.set_enabled("task_id_3"s, false);

    EXPECT_FALSE(s.is_enabled("task_id_1"s));
    EXPECT_FALSE(s.is_enabled("task_id_2"s));
    EXPECT_FALSE(s.is_enabled("task_id_3"s));
}

TEST(sstsDisable, ReEnabled)
{
    ssts::task_scheduler s(4);

    s.in("task_id_1"s, 2s, []{std::cout << "Hello A!" << std::endl;});
    s.in("task_id_2"s, 2s, []{std::cout << "Hello B!" << std::endl;});
    s.in("task_id_3"s, 2s, []{std::cout << "Hello C!" << std::endl;});

    // Disable tasks before they can run
    s.set_enabled("task_id_1"s, false);
    s.set_enabled("task_id_2"s, false);
    s.set_enabled("task_id_3"s, false);

    EXPECT_FALSE(s.is_enabled("task_id_1"s));
    EXPECT_FALSE(s.is_enabled("task_id_2"s));
    EXPECT_FALSE(s.is_enabled("task_id_3"s));

    // ReEnable tasks before they can run
    s.set_enabled("task_id_1"s, true);
    s.set_enabled("task_id_2"s, true);
    s.set_enabled("task_id_3"s, true);

    EXPECT_TRUE(s.is_enabled("task_id_1"s));
    EXPECT_TRUE(s.is_enabled("task_id_2"s));
    EXPECT_TRUE(s.is_enabled("task_id_3"s));
}

