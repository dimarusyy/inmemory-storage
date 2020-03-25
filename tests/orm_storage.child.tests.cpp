#include "include/orm_storage.h"
#include <doctest/doctest.h>

#include <array>

using namespace orm;

TEST_CASE("child_create")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 123.0);

    auto child = s.child();
    REQUIRE_EQ(std::get<0>(s.get(1))->timestamp, 123.0);
    REQUIRE_EQ(std::get<0>(child.get(1))->timestamp, 123.0);
}

TEST_CASE("child_create_empty_size")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 123.0);

    auto child = s.child();

    REQUIRE_EQ(s.size(), 1);
    REQUIRE_EQ(s.size(), child.size());
}

TEST_CASE("child_create_erase_size")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 123.0);

    auto child = s.child();
    s.erase(1);

    REQUIRE_EQ(s.size(), 0);
    REQUIRE_EQ(child.size(), 1);
}

TEST_CASE("child_modify")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 123.0);
    s.insert(2, "hello", "", 124.0);

    auto child = s.child();
    child.update(1, [](auto& el)
                 {
                     el.timestamp = 125.0;
                 });

    REQUIRE_EQ(std::get<0>(s.get(1))->timestamp, 123.0);
    REQUIRE_EQ(std::get<0>(child.get(1))->timestamp, 125.0);
}

TEST_CASE("child_commit_update_size")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 100.0);
    s.insert(2, "hello", "", 100.0);

    auto old_size = s.size();
    auto child = s.child();
    child.update(1, [](auto& el)
                 {
                     el.timestamp = 200.0;
                 });
    child.commit();
    auto new_size = s.size();

    REQUIRE_EQ(old_size, new_size);
}

TEST_CASE("child_commit_update")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 100.0);
    s.insert(2, "hello", "", 100.0);

    auto child = s.child();
    child.update(1, [](auto& el)
                 {
                     el.timestamp = 200.0;
                 });
    child.commit();

    auto obj = *(std::get<0>(s.get(1)));
    REQUIRE_EQ(obj.timestamp, 200.0);
}

TEST_CASE("child_commit_insert_size")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 100.0);
    s.insert(2, "hello", "", 100.0);

    auto child = s.child();
    child.insert(3, "hello", "", 200.0);
    child.insert(4, "hello", "", 300.0);
    child.commit();

    REQUIRE_EQ(s.size(), 4);
}

TEST_CASE("child_commit_insert")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 100.0);
    s.insert(2, "hello", "", 100.0);

    auto child = s.child();
    child.insert(3, "hello", "", 200.0);
    child.insert(4, "hello", "", 300.0);
    child.commit();

    auto obj3 = *(std::get<0>(s.get(3)));
    auto obj4 = *(std::get<0>(s.get(4)));
    REQUIRE_EQ(obj3.timestamp, 200.0);
    REQUIRE_EQ(obj4.timestamp, 300.0);
}

TEST_CASE("range_query")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 100.0);
    s.insert(2, "hello", "", 200.0);

    auto child = s.child();
    child.insert(3, "hello", "", 300.0);
    child.insert(4, "hello", "", 400.0);
    child.insert(5, "hello", "", 500.0);

    auto g = s.range_query<timestamp_tag>(100.0, 400.0);
    
    auto g_it = g.begin();
    while (g_it != g.end())
    {
        auto it = g_it->first;
        int i = 1;
        while (it != g_it->second)
        {
            auto obj = *it;
            REQUIRE_EQ(obj.id, i);
            REQUIRE_EQ(obj.timestamp, 100.0 * i);
            i++; it++;
        }
        g_it++;
    }
}

TEST_CASE("query")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 100.0);
    s.insert(2, "hello", "", 200.0);

    auto child = s.child();
    child.insert(3, "hello", "", 100.0);
    child.insert(4, "hello", "", 400.0);
    child.insert(5, "hello", "", 500.0);

    auto g = s.query<timestamp_tag>(100.0);

    std::array<int64_t, 3u> valid_answers{ 1,1,3 };

    std::size_t i = 0;

    auto g_it = g.begin();
    while (g_it != g.end())
    {
        auto it = g_it->first;
        while (it != g_it->second)
        {
            auto obj = *it;
            REQUIRE_EQ(obj.id, valid_answers[i++]);
            it++;
        }
        g_it++;
    }
}