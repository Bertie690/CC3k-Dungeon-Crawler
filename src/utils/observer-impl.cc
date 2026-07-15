module observer;

#ifdef __INTELLISENSE__
#include <type_traits>
#include <vector>

#include "observer.cc"
#endif  // __INTELLISENSE__

template <typename Events>
  requires std::is_class_v<Events>
void Subject<Events>::attach(Observer<Events>* observer) {
  observers.push_back(observer);
}

template <typename Events>
  requires std::is_class_v<Events>
void Subject<Events>::detach(Observer<Events>* observer) {
  observers.remove(observer);
}