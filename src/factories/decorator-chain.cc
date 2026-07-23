export module decoratorchain;

#ifdef __INTELLISENSE__
#include <memory>
#include <type_traits>
#else
import <memory>;
import <type_traits>;
#endif  // __INTELLISENSE__


// Utility class to chain decorators together using a fluent builder.
export template<typename Base>
  requires std::is_class_v<Base>
class DecoratorChain {
  std::unique_ptr<Base> base;

  public:
    // Create a new DecoratorChain with the given root object.
    DecoratorChain(std::unique_ptr<Base> base) : base(std::move(base)) {};

    // Add a decorator to the chain.
    // Returns the current instance for chaining.
    template<typename Decorator, typename... Args>
      requires std::is_class_v<Decorator> && std::is_base_of_v<Base, Decorator> && std::is_constructible_v<Decorator, std::unique_ptr<Base>, Args...>
    DecoratorChain<Decorator>& add(Args&&... args) {
      base = std::make_unique<Decorator>(std::move(base), std::forward<Args>(args)...);

      return *this;
    }

    // Return the final decorated object.
    std::unique_ptr<Base> build() {
      return std::move(base);
    }
};