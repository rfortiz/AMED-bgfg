
#include <gtest/gtest.h>

#include "hello.h"

TEST(Hello, get_message)
{
    std::string m = get_message();
    ASSERT_EQ("Hello, World!", m);
}



//~ frame = Mat(2, 2, CV_8UC3, Scalar(1,1,1))*128;
