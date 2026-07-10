export module decorator_chain;

#ifdef __INTELLISENSE__
#include <type_traits>
#include <memory>
#else
import <type_traits>;
import <memory>;
#endif // __INTELLISENSE__

export template <typename Base>
  requires std::is_class_v<Base> && std::is_abstract_v<Base>
class DecoratorChain final {
  using Ptr = std::unique_ptr<Base>;


 public:
  DecoratorChain() {}

  /**
   * @brief Add the given class to the base of the chain.
   * @tparam Concrete The concrete type of the class to add.
   * @returns A reference to this builder, for chaining.
   */
  template <typename Concrete>
    requires std::is_base_of_v<Base, Concrete> && std::is_class_v<Concrete> && std::is_constructible_v<Concrete, Args...>
  static DecoratorChain create() {
    return DecoratorChain([](auto&&... args) {
      return std::make_unique<Concrete>(std::forward<decltype(args)>(args)...);
    });
  }

  .
};