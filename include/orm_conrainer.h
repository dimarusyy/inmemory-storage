#pragma once

#include "config.h"
#include "orm_build.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/member.hpp>

namespace orm
{
    ORM_BUILD(Todo, id, title, description, timestamp)
    using storage_type = boost::multi_index_container<value_type, index_type>;

    using iterator = storage_type::iterator;
    using const_iterator = storage_type::const_iterator;
}