// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef PERSON_ODB_HXX
#define PERSON_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "person.hxx"

#include <memory>
#include <cstddef>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // person
  //
  template <>
  struct class_traits< ::person >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::person >
  {
    public:
    typedef ::person object_type;
    typedef ::person* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef long unsigned int id_type;

    static const bool auto_id = true;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/sqlite/version.hxx>
#include <odb/sqlite/forward.hxx>
#include <odb/sqlite/binding.hxx>
#include <odb/sqlite/sqlite-types.hxx>
#include <odb/sqlite/query.hxx>

namespace odb
{
  // email_address
  //
  template <>
  class access::composite_value_traits< ::email_address, id_sqlite >
  {
    public:
    typedef ::email_address value_type;

    struct image_type
    {
      // recipient_
      //
      details::buffer recipient_value;
      std::size_t recipient_size;
      bool recipient_null;

      // domain_
      //
      details::buffer domain_value;
      std::size_t domain_size;
      bool domain_null;
    };

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (sqlite::bind*,
          image_type&,
          sqlite::statement_kind);

    static bool
    init (image_type&,
          const value_type&,
          sqlite::statement_kind);

    static void
    init (value_type&,
          const image_type&,
          database*);

    static bool
    get_null (const image_type&);

    static void
    set_null (image_type&,
              sqlite::statement_kind);

    static const std::size_t column_count = 2UL;
  };

  // person
  //
  template <typename A>
  struct query_columns< ::person, id_sqlite, A >
  {
    // id
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        long unsigned int,
        sqlite::id_integer >::query_type,
      sqlite::id_integer >
    id_type_;

    static const id_type_ id;

    // first
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    first_type_;

    static const first_type_ first;

    // last
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        ::std::string,
        sqlite::id_text >::query_type,
      sqlite::id_text >
    last_type_;

    static const last_type_ last;

    // age
    //
    typedef
    sqlite::query_column<
      sqlite::value_traits<
        short unsigned int,
        sqlite::id_integer >::query_type,
      sqlite::id_integer >
    age_type_;

    static const age_type_ age;

    // email
    //
    struct email_class_
    {
      email_class_ ()
      {
      }

      // recipient
      //
      typedef
      sqlite::query_column<
        sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::query_type,
        sqlite::id_text >
      recipient_type_;

      static const recipient_type_ recipient;

      // domain
      //
      typedef
      sqlite::query_column<
        sqlite::value_traits<
          ::std::string,
          sqlite::id_text >::query_type,
        sqlite::id_text >
      domain_type_;

      static const domain_type_ domain;
    };

    static const email_class_ email;
  };

  template <typename A>
  const typename query_columns< ::person, id_sqlite, A >::id_type_
  query_columns< ::person, id_sqlite, A >::
  id (A::table_name, "\"id\"", 0);

  template <typename A>
  const typename query_columns< ::person, id_sqlite, A >::first_type_
  query_columns< ::person, id_sqlite, A >::
  first (A::table_name, "\"first\"", 0);

  template <typename A>
  const typename query_columns< ::person, id_sqlite, A >::last_type_
  query_columns< ::person, id_sqlite, A >::
  last (A::table_name, "\"last\"", 0);

  template <typename A>
  const typename query_columns< ::person, id_sqlite, A >::age_type_
  query_columns< ::person, id_sqlite, A >::
  age (A::table_name, "\"age\"", 0);

  template <typename A>
  const typename query_columns< ::person, id_sqlite, A >::email_class_::recipient_type_
  query_columns< ::person, id_sqlite, A >::email_class_::
  recipient (A::table_name, "\"email_recipient\"", 0);

  template <typename A>
  const typename query_columns< ::person, id_sqlite, A >::email_class_::domain_type_
  query_columns< ::person, id_sqlite, A >::email_class_::
  domain (A::table_name, "\"email_domain\"", 0);

  template <typename A>
  const typename query_columns< ::person, id_sqlite, A >::email_class_
  query_columns< ::person, id_sqlite, A >::email;

  template <typename A>
  struct pointer_query_columns< ::person, id_sqlite, A >:
    query_columns< ::person, id_sqlite, A >
  {
  };

  template <>
  class access::object_traits_impl< ::person, id_sqlite >:
    public access::object_traits< ::person >
  {
    public:
    struct id_image_type
    {
      long long id_value;
      bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // id_
      //
      long long id_value;
      bool id_null;

      // first_
      //
      details::buffer first_value;
      std::size_t first_size;
      bool first_null;

      // last_
      //
      details::buffer last_value;
      std::size_t last_size;
      bool last_null;

      // age_
      //
      long long age_value;
      bool age_null;

      // email_
      //
      composite_value_traits< ::email_address, id_sqlite >::image_type email_value;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    // nicknames_
    //
    struct nicknames_traits
    {
      static const std::size_t id_column_count = 1UL;
      static const std::size_t data_column_count = 3UL;

      static const bool versioned = false;

      static const char insert_statement[];
      static const char select_statement[];
      static const char delete_statement[];

      typedef ::names container_type;
      typedef
      odb::access::container_traits<container_type>
      container_traits_type;
      typedef container_traits_type::index_type index_type;
      typedef container_traits_type::value_type value_type;

      typedef ordered_functions<index_type, value_type> functions_type;
      typedef sqlite::container_statements< nicknames_traits > statements_type;

      struct data_image_type
      {
        // index
        //
        long long index_value;
        bool index_null;

        // value
        //
        details::buffer value_value;
        std::size_t value_size;
        bool value_null;

        std::size_t version;
      };

      static void
      bind (sqlite::bind*,
            const sqlite::bind* id,
            std::size_t id_size,
            data_image_type&);

      static void
      grow (data_image_type&,
            bool*);

      static void
      init (data_image_type&,
            index_type*,
            const value_type&);

      static void
      init (index_type&,
            value_type&,
            const data_image_type&,
            database*);

      static void
      insert (index_type, const value_type&, void*);

      static bool
      select (index_type&, value_type&, void*);

      static void
      delete_ (void*);

      static void
      persist (const container_type&,
               statements_type&);

      static void
      load (container_type&,
            statements_type&);

      static void
      update (const container_type&,
              statements_type&);

      static void
      erase (statements_type&);
    };

    // age_weight_
    //
    struct age_weight_traits
    {
      static const std::size_t id_column_count = 1UL;
      static const std::size_t data_column_count = 3UL;

      static const bool versioned = false;

      static const char insert_statement[];
      static const char select_statement[];
      static const char delete_statement[];

      typedef ::age_weight_map container_type;
      typedef
      odb::access::container_traits<container_type>
      container_traits_type;
      typedef container_traits_type::key_type key_type;
      typedef container_traits_type::value_type value_type;

      typedef map_functions<key_type, value_type> functions_type;
      typedef sqlite::container_statements< age_weight_traits > statements_type;

      struct data_image_type
      {
        // key
        //
        long long key_value;
        bool key_null;

        // value
        //
        double value_value;
        bool value_null;

        std::size_t version;
      };

      static void
      bind (sqlite::bind*,
            const sqlite::bind* id,
            std::size_t id_size,
            data_image_type&);

      static void
      grow (data_image_type&,
            bool*);

      static void
      init (data_image_type&,
            const key_type*,
            const value_type&);

      static void
      init (key_type&,
            value_type&,
            const data_image_type&,
            database*);

      static void
      insert (const key_type&, const value_type&, void*);

      static bool
      select (key_type&, value_type&, void*);

      static void
      delete_ (void*);

      static void
      persist (const container_type&,
               statements_type&);

      static void
      load (container_type&,
            statements_type&);

      static void
      update (const container_type&,
              statements_type&);

      static void
      erase (statements_type&);
    };

    using object_traits<object_type>::id;

    static id_type
    id (const id_image_type&);

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          bool*);

    static void
    bind (sqlite::bind*,
          image_type&,
          sqlite::statement_kind);

    static void
    bind (sqlite::bind*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          sqlite::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef sqlite::object_statements<object_type> statements_type;

    typedef sqlite::query_base query_base_type;

    static const std::size_t column_count = 6UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::person, id_common >:
    public access::object_traits_impl< ::person, id_sqlite >
  {
  };

  // person
  //
}

#include "person-odb.ixx"

#include <odb/post.hxx>

#endif // PERSON_ODB_HXX
