export module colors;

#ifdef __INTELLISENSE__
#include <string>
#else
import <string>;
#endif  // __INTELLISENSE__

export namespace Color {
  const std::string RESET = "\033[0m";
  const std::string BOLD = "\033[1m";
  const std::string RED = "\033[31m";
  const std::string GREEN = "\033[32m";
  const std::string YELLOW = "\033[33m";
  const std::string BLUE = "\033[34m";
  const std::string MAGENTA = "\033[35m";
  const std::string CYAN = "\033[36m";

  // Apply a color to the given text.
  std::string colorize(const std::string& color, const std::string& text) {
    return color + text + RESET;
  }
}  // namespace Color