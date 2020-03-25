#pragma once

#include "config.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/member.hpp>

#define INDEX_UNIQUE(tag_name,type_name,field_id,ptr_id) \
    boost::multi_index::ordered_unique< \
        boost::multi_index::tag<tag_name>, boost::multi_index::member<type_name, decltype(field_id), ptr_id> \
    >

#define INDEX_NON_UNIQUE(tag_name,type_name,field_id,ptr_id) \
    boost::multi_index::ordered_non_unique< \
        boost::multi_index::tag<tag_name>, boost::multi_index::member<type_name, decltype(field_id), ptr_id> \
    >

namespace orm
{
    struct build
    {
        // should be reflected and be template argument
        using value_type = Todo;

        using storage_type = boost::multi_index_container<
            value_type,
            boost::multi_index::indexed_by<
            INDEX_UNIQUE(id_tag, value_type, value_type::id, &value_type::id),
            INDEX_NON_UNIQUE(title_tag, value_type, value_type::title, &value_type::title),
            INDEX_NON_UNIQUE(description_tag, value_type, value_type::description, &value_type::description),
            INDEX_NON_UNIQUE(timestamp_tag, value_type, value_type::timestamp, &value_type::timestamp)
            >
        >;

        using iterator = storage_type::iterator;
        using const_iterator = storage_type::const_iterator;
    };
}