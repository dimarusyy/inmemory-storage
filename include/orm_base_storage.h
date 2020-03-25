#include "error.h"
#include "orm_conrainer.h"

#include <boost/lambda/lambda.hpp>

namespace orm
{
    enum class MergeStrategy
    {
        auto_merge  = 1, // do either insert or update depending on whether entry is in storage
        insert_only = 2, // insert the new entries only
        update_only = 3  // update the existing entries only
    };

    struct orm_base_storage_t
    {
        // aliases
        using value_type = orm::build::value_type;
        using storage_type = orm::build::storage_type;
        using iterator = orm::build::iterator;
        using const_iterator = orm::build::const_iterator;

        orm_base_storage_t() = default;

        orm_base_storage_t(storage_type store)
            : _storage(std::move(store))
        {
        }

        using index_type = decltype(value_type::id);

        error_t insert(const value_type& obj)
        {
            auto rc = _storage.insert(obj).second ? error_t::success : error_t::duplicate;
            return rc;
        }

        template <typename...Args>
        error_t insert(int64_t id, Args...args)
        {
            value_type obj{ id, args... };
            return insert(obj);
        }

        error_t erase(int64_t id)
        {
            return _storage.erase(id) != 0 ? error_t::success : error_t::not_found;
        }

        auto get(index_type id)
        {
            auto& index = _storage.template get<id_tag>();
            return std::make_tuple(index.find(id), index.end());
        }

        template <typename Tag, typename IndexType>
        auto query(const IndexType& value)
        {
            auto& index = _storage.template get<Tag>();
            return index.equal_range(value);
        }

        template <typename Tag, typename IndexType>
        auto range_query(const IndexType& first, const IndexType& last)
        {
            auto& index = _storage.template get<Tag>();
            return index.range(first <= boost::lambda::_1, boost::lambda::_1 <= last);
        }

        template <typename...ModifyArgs>
        auto update(index_type id, ModifyArgs...args)
        {
            auto& index = _storage.template get<id_tag>();
            auto it = index.find(id);
            if (it != index.end())
            {
                _storage.modify(it, std::move(args)...);
                return error_t::success;
            }
            return error_t::not_found;
        }

        auto size()
        {
            return _storage.size();
        }

        void merge(const orm_base_storage_t& other_storage, MergeStrategy strategy = MergeStrategy::auto_merge)
        {
            for (auto entry : other_storage.storage())
            {
                auto it_tuple = get(entry.id);
                auto& first = std::get<0>(it_tuple);
                auto& last = std::get<1>(it_tuple);
                if (first == last)
                {
                    // insert the new element
                    if(strategy != MergeStrategy::update_only)
                        insert(entry);
                }
                else
                {
                    // replace element with the new one
                    if (strategy != MergeStrategy::insert_only)
                        _storage.replace(first, entry) ? error_t::success : error_t::failure;
                }
            }
        }

        const storage_type& storage() const
        {
            return _storage;
        }

    protected:
        storage_type _storage;
    };
}