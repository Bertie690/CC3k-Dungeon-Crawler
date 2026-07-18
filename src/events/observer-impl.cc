module observer;

#ifdef __INTELLISENSE__
#include <type_traits>
#include <vector>

#include "observer.cc"
#endif  // __INTELLISENSE__

template <typename Event>
  requires std::is_class_v<Event>
void Subject<Event>::notify(const Event& event) {
  for (ObserverFor<Event>* observer : observers) {
    observer->onNotify(event);
  }
}

template <typename Event>
  requires std::is_class_v<Event>
void Subject<Event>::attach(ObserverFor<Event>* observer) {
  observers.push_back(observer);
}

template <typename Event>
  requires std::is_class_v<Event>
void Subject<Event>::detach(ObserverFor<Event>* observer) {
  observers.remove(observer);
}
