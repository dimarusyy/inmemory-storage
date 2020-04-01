#pragma once

#include <boost/preprocessor.hpp>

#define ORM_TAG _tag

#define INDEX_UNIQUE(tag_name,type_name,field_id,ptr_id) \
    boost::multi_index::ordered_unique<\
        boost::multi_index::tag<tag_name>, boost::multi_index::member<type_name, decltype(field_id), ptr_id>\
    >

#define INDEX_NON_UNIQUE(tag_name,type_name,field_id,ptr_id) \
    boost::multi_index::ordered_non_unique<\
        boost::multi_index::tag<tag_name>, boost::multi_index::member<type_name, decltype(field_id), ptr_id>\
    >

#define ORM_VALIST_HEAD(...) BOOST_PP_SEQ_HEAD(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define ORM_VALIST_TAIL(...) BOOST_PP_SEQ_TAIL(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define ORM_CREATE_TAG(r, data, i, elem) \
    BOOST_PP_CAT(struct elem, ORM_TAG{ \
        using index_unique_t = INDEX_UNIQUE( \
            BOOST_PP_CAT(elem,ORM_TAG), \
            data, \
            BOOST_PP_CAT(data::,elem), \
            BOOST_PP_CAT(&data::,elem) \
        );\
        using index_non_unique_t = INDEX_NON_UNIQUE( \
            BOOST_PP_CAT(elem,ORM_TAG), \
            data, \
            BOOST_PP_CAT(data::,elem), \
            BOOST_PP_CAT(&data::,elem) \
        );\
        using index_t = BOOST_PP_IF(i,index_non_unique_t,index_unique_t);\
    };\
)

#define ORM_CREATE_TAGS(...) \
    BOOST_PP_SEQ_FOR_EACH_I(ORM_CREATE_TAG,ORM_VALIST_HEAD(__VA_ARGS__),ORM_VALIST_TAIL(__VA_ARGS__))

#define ORM_CREATE_INDEX(r, data, elem) \
    BOOST_PP_CAT(typename BOOST_PP_CAT(elem, ORM_TAG),::index_t)

#define ORM_CREATE_INDEXES(...) \
    boost::multi_index::indexed_by<\
        BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(ORM_CREATE_INDEX,ORM_VALIST_HEAD(__VA_ARGS__),ORM_VALIST_TAIL(__VA_ARGS__)))\
    >

#define ORM_BUILD(...)\
    ORM_CREATE_TAGS(__VA_ARGS__);\
    using value_type = ORM_VALIST_HEAD(__VA_ARGS__);\
    using index_type = ORM_CREATE_INDEXES(__VA_ARGS__);