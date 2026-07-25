export module variantcast;

#ifdef __INTELLISENSE__
#include <type_traits>
#include <variant>
#else
import <type_traits>;
import <variant>;
#endif  // __INTELLISENSE__

template <typename Variant>
concept IsVariant = requires(Variant v) { typename std::variant_size<Variant>::type; };

// Utility function to cast a std::variant to another std::variant type, if possible.
// Throws if the types cannot be converted directly.
export template <typename To, typename From>
  requires IsVariant<To> && IsVariant<From>
To variantCast(const From& from) {
  return std::visit(
      [](const auto& value) -> To {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_constructible_v<To, T>) {
          return To{value};
        } else {
          throw std::bad_variant_access{};
        }
      },
      from);
}
