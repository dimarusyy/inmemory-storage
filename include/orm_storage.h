#pragma once

#include "orm_base_storage.h"
#include "cppcoro/generator.hpp"

#include <optional>
#include <vector>

namespace orm
{
    struct orm_storage_t final
    {
        using iterator = orm_base_storage_t::iterator;

        orm_storage_t()
            : _storage(std::make_shared<orm_base_storage_t>())
        {
        }

        orm_storage_t(std::shared_ptr<orm_base_storage_t> storage, std::shared_ptr<orm_base_storage_t> parent = nullptr)
            : _storage(std::move(storage)), _parent(parent)
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

        auto get(orm_base_storage_t::index_type id)
        {
            return _storage->get(id);
        }

        template <typename Tag, typename IndexType>
        cppcoro::generator<std::pair<iterator, iterator>> query(const IndexType& value)
        {
            co_yield _storage->query<Tag>(value);
            for (auto& s : _siblings)
            {
                auto child = s.lock();
                if (child)
                {
                    co_yield child->query<Tag>(value);
                }
            }
        }

        template <typename Tag, typename IndexType>
        cppcoro::generator<std::pair<iterator, iterator>> range_query(const IndexType& first,
                                                                      const IndexType& last)
        {
            co_yield _storage->range_query<Tag>(first, last);
            for (auto& s : _siblings)
            {
                auto child = s.lock();
                if (child)
                {
                    co_yield child->range_query<Tag>(first, last);
                }
            }
        }

        template <typename...ModifyArgs>
        auto update(orm_base_storage_t::index_type id, ModifyArgs...args)
        {
            return _storage->update(id, args...);
        }

        auto size()
        {
            return _storage->size();
        }

        error_t commit()
        {
            // 1) should we merge based on timestamp ? merge strategy ?
            // 2) should we consider children state before merging ?
            auto parent_storage = _parent.lock();
            if (!parent_storage)
                return error_t::not_found;
            return parent_storage->merge(*_storage);
        }

        orm_storage_t child()
        {
            const auto& it = _siblings.insert(_siblings.end(), std::weak_ptr<orm_base_storage_t>());
            
            // make a copy of own storage
            std::shared_ptr<orm_base_storage_t> new_storage(new orm_base_storage_t(_storage->storage()),
                                                            [this, it](auto* ptr)
                                                            {
                                                                delete ptr;
                                                                _siblings.erase(it);
                                                            });
            *it = new_storage;
            return orm_storage_t{ new_storage, _storage};
        }

    private:
        std::shared_ptr<orm_base_storage_t> _storage;

        std::weak_ptr<orm_base_storage_t> _parent;
        std::list<std::weak_ptr<orm_base_storage_t>> _siblings;
    };
}
