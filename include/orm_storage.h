#pragma once

#include "error.h"
#include "orm_conrainer.h"

#include <boost/lambda/lambda.hpp>

#include <optional>
#include <tuple>
#include <utility>

namespace orm
{
    struct orm_storage_base_t
    {
        // aliases
        using value_type = orm::build::value_type;
        using storage_type = orm::build::storage_type;

        orm_storage_base_t() = default;

        orm_storage_base_t(storage_type store)
            : _store(std::move(store))
        {
        }

        using index_type = decltype(value_type::id);

        template <typename...Args>
        error_t insert(int64_t id, Args...args)
        {
            value_type obj{ id, args... };
            return _store.insert(obj).second ? error_t::success : error_t::duplicate;
        }

        error_t erase(int64_t id)
        {
            return _store.erase(id) != 0 ? error_t::success : error_t::not_found;
        }

        auto get(index_type id)
        {
            auto& index = _store.template get<id_tag>();
            return std::make_tuple(index.find(id), index.end());
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

        template <typename...ModifyArgs>
        auto update(index_type id, ModifyArgs...args)
        {
            auto& index = _store.template get<id_tag>();
            auto it = index.find(id);
            if (it != index.end())
            {
                _store.modify(it, std::move(args)...);
                return error_t::success;
            }
            return error_t::not_found;
        }

        auto size()
        {
            return _store.size();
        }

    protected:
        storage_type _store;
    };


    struct orm_storage_t final : orm_storage_base_t
    {
        using orm_storage_base_t::value_type;
        using orm_storage_base_t::storage_type;
        using orm_storage_base_t::orm_storage_base_t;

        orm_storage_t(orm_storage_base_t::storage_type storage, orm_storage_t& parent)
            : orm_storage_base_t(std::move(storage)), _parent(std::ref(parent))
        {
        }

        orm_storage_t child()
        {
            return orm_storage_t(orm_storage_base_t::_store, *this);
        }

    private:
        std::optional<std::reference_wrapper<orm_storage_t>> _parent;
    };

}
