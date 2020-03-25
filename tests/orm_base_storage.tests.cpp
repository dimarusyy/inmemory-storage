#include "include/orm_base_storage.h"
#include <doctest/doctest.h>

using namespace orm;

TEST_CASE("create")
{
    REQUIRE_NOTHROW(orm_base_storage_t{});
}

TEST_CASE("insert")
{
    orm_base_storage_t s{};
    s.insert(0);
    s.insert(1, "some title1");
    s.insert(2, "some title2", "some description2");
    s.insert(3, "some title3", "some description3", 123456.0f);

    REQUIRE_EQ(s.size(), 4);
}

TEST_CASE("erase")
{
    orm_base_storage_t s{};
    s.insert(0);
    s.insert(1, "some title1");
    s.insert(2, "some title2", "some description2");
    s.insert(3, "some title3", "some description3", 123456.0f);


    s.erase(0);
    REQUIRE_EQ(s.size(), 3);

    s.erase(1);
    REQUIRE_EQ(s.size(), 2);

    s.erase(2);
    REQUIRE_EQ(s.size(), 1);

    s.erase(3);
    REQUIRE_EQ(s.size(), 0);
}

TEST_CASE("get_non_existing")
{
    orm_base_storage_t s{};
    s.insert(1, "hello");
    auto rc = s.get(1);

    REQUIRE_NE(std::get<0>(rc), std::get<1>(rc));

    auto obj = *(std::get<0>(rc));
    REQUIRE_EQ(obj.id, 1);
    REQUIRE_EQ(obj.title, "hello");
}

TEST_CASE("get_non_existing")
{
    orm_base_storage_t s{};
    auto rc = s.get(1);
    REQUIRE_EQ(std::get<0>(rc), std::get<1>(rc));
}

TEST_CASE("query")
{
    orm_base_storage_t s{};
    s.insert(1, "hello");
    s.insert(2, "hello1");
    s.insert(3, "hello2");
    
    auto rc = s.query<title_tag>("hello");

    auto first = std::get<0>(rc);
    auto last = std::get<1>(rc);
    
    auto obj = *first;
    REQUIRE_EQ(obj.id, 1);
    REQUIRE_EQ(obj.title, "hello");
    
    first++;
    REQUIRE_EQ(first, last);
}

TEST_CASE("query_range")
{
    orm_base_storage_t s{};
    s.insert(1, "hello", "", 123.0);
    s.insert(2, "hello", "", 126.0);
    s.insert(3, "hello", "", 125.0);
    s.insert(4, "hello", "", 126.0);

    auto rc = s.range_query<timestamp_tag>(123, 125);

    auto first = std::get<0>(rc);
    auto last = std::get<1>(rc);

    REQUIRE_EQ(first->id, 1); first++;
    REQUIRE_EQ(first->id, 3); first++;

    REQUIRE_EQ(first, last);
}

TEST_CASE("update_title")
{
    orm_base_storage_t s{};
    s.insert(1, "hello", "", 123.0);
    s.insert(2, "hello", "", 126.0);
    s.insert(3, "hello", "", 125.0);
    s.insert(4, "hello", "", 126.0);

    s.update(2, [](auto& el)
             {
                 el.title = "hello updated";
             });
    REQUIRE_EQ(std::get<0>(s.get(2))->title, "hello updated");
}

TEST_CASE("update_and rollback")
{
    orm_base_storage_t s{};
    s.insert(1, "hello", "", 123.0);
    s.insert(2, "hello", "", 124.0);
    s.insert(3, "hello", "", 125.0);
    s.insert(4, "hello", "", 126.0);

    s.update(2, [](auto& el)
             {
                 el.title = "hello new";
             },
             [](auto& el)
             {
                 el.title = "hello old";
             });
    REQUIRE_EQ(std::get<0>(s.get(2))->title, "hello new");
}
