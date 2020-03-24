#pragma once

#include "config.h"
#include "error.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/lambda/lambda.hpp>

#include <tuple>


using boost::multi_index_container;
using namespace boost::multi_index;

#define INDEX_UNIQUE(tag_name,type_name,field_id,ptr_id) \
    ordered_unique<tag<tag_name>, member<type_name, decltype(field_id), ptr_id>>

#define INDEX_NON_UNIQUE(tag_name,type_name,field_id,ptr_id) \
    ordered_non_unique<tag<tag_name>, member<type_name, decltype(field_id), ptr_id>>

template <typename T>
struct orm_storage_t
{
    typedef boost::multi_index_container<
        T,
        indexed_by<
        INDEX_UNIQUE(id_tag, T, T::id, &T::id),
        INDEX_NON_UNIQUE(title_tag, T, T::title, &T::title),
        INDEX_NON_UNIQUE(description_tag, T, T::description, &T::description),
        INDEX_NON_UNIQUE(timestamp_tag, T, T::timestamp, &T::timestamp)
        >
    > storage_type;

    orm_storage_t() = default;

    orm_storage_t(storage_type store)
        : _store(std::move(store))
    {
    }

    using index_type = decltype(T::id);

    template <typename...Args>
    error_t insert(int64_t id, Args...args)
    {
        T obj{ id, args... };
        return _store.insert(obj).second ? error_t::success : error_t::duplicate;
    }

    error_t erase(int64_t id)
    {
        _store.erase(id);
        return error_t::success;
    }

    auto get(index_type id)
    {
        auto& id_index = _store.template get<id_tag>();
        return std::make_tuple(id_index.find(id), id_index.end());
    }

    template <typename Tag, typename IndexType>
    auto query(const IndexType& value)
    {
        auto& index = _store.template get<Tag>();
        return index.equal_range(value);
    }

    template <typename Tag, typename IndexType>
    auto query_range(const IndexType& first,
                     const IndexType& last)
    {
        auto& index = _store.template get<Tag>();
        return index.range(first <= boost::lambda::_1, boost::lambda::_1 <= last);
    }

    template <typename IndexType, typename Handler, typename Rollback>
    void update_or_rollback(const IndexType& value, Handler handler, Rollback rollback)
    {
        auto& index = _store.template get<id_tag>();
        auto [first, last] = index.equal_range(value);
        while (first != last)
        {
            _store.modify(first, std::move(handler), std::move(rollback));
            first++;
        }
    }

    template <typename IndexType, typename Handler>
    void update(const IndexType& value, Handler handler)
    {
        auto& index = _store.template get<id_tag>();
        auto [first, last] = index.equal_range(value);
        while (first != last)
        {
            _store.modify(first, std::move(handler));
            first++;
        }
    }

    auto size()
    {
        return _store.size();
    }

    orm_storage_t<T> child() const
    {
        return { _store };
    }

private:
    storage_type _store;
};

#undef INDEX_UNIQUE
#undef INDEX_NON_UNIQUE