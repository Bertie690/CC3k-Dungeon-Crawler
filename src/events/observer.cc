export module observer;

#pragma once

#ifdef __INTELLISENSE__
#include <list>
#include <type_traits>
#else
import <type_traits>;
import <list>;
#endif  // __INTELLISENSE__

// forward declare subject for friendship

export template <typename Event>
  requires std::is_class_v<Event>
class SubjectFor;

// Interface representing an observer that tracks a single event type.
template <typename Event>
  requires std::is_class_v<Event>
class ObserverFor {
  friend class SubjectFor<Event>;
  // Trigger effects upon an event of the given type being emitted.
  virtual void onNotify(const Event& event) = 0;

 public:
  virtual ~ObserverFor() = default;
};

// Interface representing an observer that tracks one or more event types.
export template <typename... Event>
  requires(std::is_class_v<Event> && ...) && (sizeof...(Event) > 0)
class Observer : public ObserverFor<Event>... {
 public:
  virtual ~Observer() = default;
};

// Class representing a subject that can emit a single event type.
template <typename Event>
  requires std::is_class_v<Event>
class SubjectFor {
  // The backing list of observers.
  std::list<ObserverFor<Event>*> observers;

 protected:
  // Notify all attached observers of a corresponding event being emitted.
  void notify(const Event& event);

 public:
  // Attach a (compatibly-typed) observer to this Subject, which will be notified upon a corresponding event being emitted.
  //
  // The Subject does not take ownership of the observer; it is the caller's responsibility to ensure the Observer
  // lives longer than the duration of its attachment.
  void attach(ObserverFor<Event>* observer);
  // Detach an observer from this Subject, which will no longer be notified upon a corresponding event being emitted.
  void detach(ObserverFor<Event>* observer);

  virtual ~SubjectFor() = default;
};

// Class representing a subject that can emit multiple event types.
export template <typename... Events>
  requires(std::is_class_v<Events> && ...) && (sizeof...(Events) > 0)
class Subject : public SubjectFor<Events>... {
  virtual ~Subject() = default;
};

template <typename Event>
  requires std::is_class_v<Event>
void SubjectFor<Event>::notify(const Event& event) {
  for (ObserverFor<Event>* observer : observers) {
    observer->onNotify(event);
  }
}

template <typename Event>
  requires std::is_class_v<Event>
void SubjectFor<Event>::attach(ObserverFor<Event>* observer) {
  observers.push_back(observer);
}

template <typename Event>
  requires std::is_class_v<Event>
void SubjectFor<Event>::detach(ObserverFor<Event>* observer) {
  observers.remove(observer);
}
