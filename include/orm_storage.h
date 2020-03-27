#pragma once

#include "orm_base_storage.h"
#include "cppcoro/generator.hpp"

#include <optional>
#include <vector>

namespace orm
{
    struct proxy_storage_t
    {
        using storage_type = base_storage_t::storage_type;

        proxy_storage_t()
            : _storage(std::make_shared<base_storage_t>()), _parent({})
        {
        }

        proxy_storage_t(std::shared_ptr<base_storage_t> storage, std::shared_ptr<base_storage_t> parent = nullptr)
            : _storage(std::move(storage)), _parent(std::move(parent))
        {
        }

        template <typename...Args>
        error_t insert(int64_t id, Args...args)
        {
            return _storage->insert(id, args...);
        }

        error_t erase(int64_t id)
        {
            return _storage->erase(id);
        }

        auto get(base_storage_t::index_type id) const
        {
            return _storage->get(id);
        }
       
        template <typename...ModifyArgs>
        auto update(base_storage_t::index_type id, ModifyArgs...args)
        {
            return _storage->update(id, args...);
        }

        auto size()
        {
            return _storage->size();
        }

        error_t commit(MergeStrategy strategy = MergeStrategy::auto_merge)
        {
            // 1) should we merge based on timestamp ? merge strategy ?
            // 2) should we consider children state before merging ?
            auto parent_storage = _parent.lock();
            if (!parent_storage)
                return error_t::not_found;
            parent_storage->merge(*_storage);
            return error_t::success;
        }

        auto storage() const
        {
            return _storage;
        }

    private:
        std::shared_ptr<base_storage_t> _storage;
        std::weak_ptr<base_storage_t> _parent;
    };

    template <typename T>
    struct storage_child_delegate_t
    {
        auto child(std::shared_ptr<base_storage_t> parent_storage)
        {
            const auto& it = _siblings.insert(_siblings.end(), std::weak_ptr<base_storage_t>());

            // make a copy of own storage
            std::shared_ptr<base_storage_t> new_storage(new base_storage_t(parent_storage->storage()),
                                                        [this, it](auto* ptr)
                                                        {
                                                            delete ptr;
                                                            _siblings.erase(it);
                                                        });
            *it = new_storage;
            return T(new_storage, parent_storage);
        }

        const auto& siblings() const
        {
            return _siblings;
        }

    private:
        std::list<std::weak_ptr<base_storage_t>> _siblings;
    };

    struct storage_t : proxy_storage_t
    {   
        using proxy_storage_t::proxy_storage_t;

        storage_t(storage_child_delegate_t<storage_t> child_delegate = {})
            : _child_delegate(child_delegate)
        {
        }

        template <typename Tag, typename IndexType,
            typename ReturnType = std::pair<typename storage_type::index<Tag>::type::iterator, typename storage_type::index<Tag>::type::iterator>>
            cppcoro::generator<ReturnType> query(const IndexType& value) const
        {
            co_yield storage()->query<Tag>(value);
            for (auto& s : _child_delegate.siblings())
            {
                auto child = s.lock();
                if (child)
                {
                    co_yield child->query<Tag>(value);
                }
            }
        }

        template <typename Tag, typename IndexType,
            typename ReturnType = std::pair<typename storage_type::index<Tag>::type::iterator, typename storage_type::index<Tag>::type::iterator>>
            cppcoro::generator<ReturnType> range_query(const IndexType& first,
                                                       const IndexType& last) const
        {
            co_yield storage()->range_query<Tag>(first, last);
            for (auto& s : _child_delegate.siblings())
            {
                auto child = s.lock();
                if (child)
                {
                    co_yield child->range_query<Tag>(first, last);
                }
            }
        }

        storage_t child()
        {
            return _child_delegate.child(proxy_storage_t::storage());
        }

    private:
        storage_child_delegate_t<storage_t> _child_delegate{};
    };
}
