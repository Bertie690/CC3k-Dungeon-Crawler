export module actionlog;

#pragma once

#ifdef __INTELLISENSE__
#include <string>
#else
import <string>;
#endif  // __INTELLISENSE__

export class ActionLog {
  std::string message;

 public:
  explicit ActionLog(const std::string& initialMessage = "") : message(initialMessage) {}

  void clear() { message.clear(); }

  void set(const std::string& newMessage) { message = newMessage; }

  void append(const std::string& newMessage) {
    if (!message.empty()) message += "; ";
    message += newMessage;
  }

  bool empty() const { return message.empty(); }

  const std::string& text() const { return message; }
};
