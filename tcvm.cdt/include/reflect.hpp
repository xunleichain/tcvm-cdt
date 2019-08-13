#ifndef _REFLECT_H_
#define _REFLECT_H_

#include <tuple>
#include <type_traits>

namespace tc{
namespace reflect {

template <typename Fn, typename Tuple, std::size_t... I>
inline constexpr void ForEachTuple(Tuple&& tuple,
                                   Fn&& fn,
                                   std::index_sequence<I...>) {
  using Expander = int[];
  (void)Expander{0, ((void)fn(std::get<I>(std::forward<Tuple>(tuple))), 0)...};
}

template <typename Fn, typename Tuple>
inline constexpr void ForEachTuple(Tuple&& tuple, Fn&& fn) {
  ForEachTuple(
      std::forward<Tuple>(tuple), std::forward<Fn>(fn),
      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

template <typename T>
struct is_field_pointer : std::false_type {};

template <typename C, typename T>
struct is_field_pointer<T C::*> : std::true_type {};

template <typename T>
constexpr auto is_field_pointer_v = is_field_pointer<T>::value;

}  // namespace reflect
}  // namespace tc


using namespace tc;
template <typename T>
inline constexpr auto StructSchema() {
//static_assert(false, "Nil Struct");
  return std::make_tuple();
}

#define TC_STRUCT(Struct, ...)        \
  template <>                                    \
  inline constexpr auto StructSchema<Struct>() { \
    using _Struct = Struct;                      \
    return std::make_tuple(__VA_ARGS__);         \
  }

#define TC_FIELD_NAME(StructField, FieldName) \
  std::make_tuple(&_Struct::StructField, FieldName)

template <typename T, typename Fn>
inline constexpr void ForEachField(T&& value, Fn&& fn) {
  constexpr auto struct_schema = StructSchema<std::decay_t<T>>();
  static_assert(std::tuple_size<decltype(struct_schema)>::value != 0,
               "StructSchema<T>() for type T should be specialized to return "
                "FieldSchema tuples, like ((&T::field, field_name), ...)");

  reflect::ForEachTuple(struct_schema, [&value, &fn](auto&& field_schema) {
    using FieldSchema = std::decay_t<decltype(field_schema)>;
    static_assert(
        std::tuple_size<FieldSchema>::value >= 2 &&
            reflect::is_field_pointer_v<std::tuple_element_t<0, FieldSchema>>,
        "FieldSchema tuple should be (&T::field, field_name)");

    fn(value.*(std::get<0>(std::forward<decltype(field_schema)>(field_schema))),
       std::get<1>(std::forward<decltype(field_schema)>(field_schema)));
  });
}

#endif  // _REFLECT_H_
