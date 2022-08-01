#include "ip_filter.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(IPFilter, Sorting) {
    ipv4_vec ip_pool = {
        {222, 130, 177,  64},
        {  1,  29, 168, 152},
        {222,  82, 198,  61},
        {  1,  70,  44, 170},
        {  1,   1, 234,   8},
        {222, 173, 235, 246},
    };

    ipv4_vec ip_pool_ascending = {

        {  1,   1, 234,   8},
        {  1,  29, 168, 152},
        {  1,  70,  44, 170},
        {222,  82, 198,  61},
        {222, 130, 177,  64},
        {222, 173, 235, 246},
    };

    ipv4_vec ip_pool_descending = {
        {222, 173, 235, 246},
        {222, 130, 177,  64},
        {222,  82, 198,  61},
        {  1,  70,  44, 170},
        {  1,  29, 168, 152},
        {  1,   1, 234,   8},
    };

    // ascending sort
    sort(ip_pool, true);
    EXPECT_EQ(ip_pool.size(), ip_pool_ascending.size()) << "Some ip addresses were lost while ascending sorting";
    for (size_t i = 0; i < ip_pool.size(); i++) {
        const auto& cur = ip_pool[i];
        const auto& ref = ip_pool_ascending[i];
        EXPECT_THAT(cur, testing::ContainerEq(ref));
    }

    // descending sort
    sort(ip_pool, false);
    EXPECT_EQ(ip_pool.size(), ip_pool_descending.size()) << "Some ip addresses were lost while descending sorting";
    for (size_t i = 0; i < ip_pool.size(); i++) {
        const auto& cur = ip_pool[i];
        const auto& ref = ip_pool_descending[i];
        EXPECT_THAT(cur, testing::ContainerEq(ref));
    }
}


TEST(IPFilter, FilterMaskVals) {
    ipv4_vec ip_pool = {
        {222, 130, 177,  64},
        {  1,  29, 168, 152},
        {222,  82, 198,  61},
        {  1,  70,  44, 170},
        {  1,   1, 234,   8},
        {222, 173, 235, 246},
        {  1,  87, 203, 225},
        {  1, 231,  69,  33},
        { 10,  61,  44,  33},
        {198,  82,  29,  33},
        { 29,  44,  82,   1},
        { 11, 291, 129, 111},
    };

    ipv4_vec ip_pool_1st_by_1 = {
        {  1,  29, 168, 152},
        {  1,  70,  44, 170},
        {  1,   1, 234,   8},
        {  1,  87, 203, 225},
        {  1, 231,  69,  33},
    };

    ipv4_vec ip_pool_2nd_3rd_by_29  = {
        {  1,  29, 168, 152},
        {198,  82,  29,  33},
    };

    ipv4_vec ip_pool_all_by_1  = {
        {  1,  29, 168, 152},
        {  1,  70,  44, 170},
        {  1,   1, 234,   8},
        {  1,  87, 203, 225},
        {  1, 231,  69,  33},
        { 29,  44,  82,   1},
    };

    // filter 1st byte by 1
    ipv4_vec ip_pool_sorted_1 =  filter(ip_pool, {1,0,0,0}, 1);
    EXPECT_EQ(ip_pool_sorted_1.size(), ip_pool_1st_by_1.size()) << "Filtered number of ip addresses does not coincide with reference";
    for (size_t i = 0; i < ip_pool_sorted_1.size(); i++) {
        const auto& cur = ip_pool_sorted_1[i];
        const auto& ref = ip_pool_1st_by_1[i];
        EXPECT_THAT(cur, testing::ContainerEq(ref));
    }

    // filter 2nd and 3rd byte by 29
    ipv4_vec ip_pool_sorted_2 =  filter(ip_pool, {0,1,1,0}, 29);
    EXPECT_EQ(ip_pool_sorted_2.size(), ip_pool_2nd_3rd_by_29.size()) << "Filtered number of ip addresses does not coincide with reference";
    for (size_t i = 0; i < ip_pool_sorted_2.size(); i++) {
        const auto& cur = ip_pool_sorted_2[i];
        const auto& ref = ip_pool_2nd_3rd_by_29[i];
        EXPECT_THAT(cur, testing::ContainerEq(ref));
    }

    // filter all bytes by 1
    ipv4_vec ip_pool_sorted_3 =  filter(ip_pool, {1,1,1,1}, 1);
    EXPECT_EQ(ip_pool_sorted_3.size(), ip_pool_all_by_1.size()) << "Filtered number of ip addresses does not coincide with reference";
    for (size_t i = 0; i < ip_pool_sorted_3.size(); i++) {
        const auto& cur = ip_pool_sorted_3[i];
        const auto& ref = ip_pool_all_by_1[i];
        EXPECT_THAT(cur, testing::ContainerEq(ref));
    }
}


TEST(IPFilter, FilterPositions) {
    ipv4_vec ip_pool = {
        {222, 130, 177,  64},
        {  1,  29, 168, 152},
        {222,  82, 198,  61},
        {  1,  70,  44, 170},
        {  1,   1, 234,   8},
        {222, 173, 235, 246},
        {  1,  87, 203, 225},
        {  1,  29,  33,  29},
        {  1, 231,  69,  33},
        { 10,  61,  44,  33},
        {198,  82,  29,  33},
        { 29,  44,  82,   1},
        { 11, 291, 129, 111},
        {  1,  29, 129, 225},
    };

    ipv4_vec ip_pool_sorted_ref = {
        {  1,  29, 168, 152},
        {  1,  29,  33,  29},
        {  1,  29, 129, 225},
    };

    // filter 1st and 2nd bytes by 1 and 29 accordingly
    ipv4_vec ip_pool_sorted =  filter_positions(ip_pool, {1, 29, -1, -1});
    EXPECT_EQ(ip_pool_sorted.size(), ip_pool_sorted_ref.size()) << "Filtered number of ip addresses does not coincide with reference";
    for (size_t i = 0; i < ip_pool_sorted.size(); i++) {
        const auto& cur = ip_pool_sorted[i];
        const auto& ref = ip_pool_sorted_ref[i];
        EXPECT_THAT(cur, testing::ContainerEq(ref));
    }
}