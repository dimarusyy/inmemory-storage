#include "include/orm_storage.h"
#include <doctest/doctest.h>

using namespace orm;

TEST_CASE("child_create")
{
    orm_storage_t s{};
    s.insert(1, "hello", "", 123.0);

    auto child = s.child();
    REQUIRE_EQ(std::get<0>(s.get(1))->timestamp, 123.0);
    REQUIRE_EQ(std::get<0>(child.get(1))->timestamp, 123.0);
}

TEST_CASE("child_commit")
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