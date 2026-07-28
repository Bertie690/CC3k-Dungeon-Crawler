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

 public:
  // Trigger effects upon an event of the given type being emitted.
  virtual void onNotify(const Event& event) = 0;

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
  // The backing list of observers, none of which are owned.
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
//
// Subclasses that want to subscribe to specific events can use the attach/detach methods on SubjectFor instead.
export template <typename... Events>
  requires(std::is_class_v<Events> && ...) && (sizeof...(Events) > 0)
class Subject : public SubjectFor<Events>... {
  template <typename Event, typename ObserverType>
  // Whether the given ObserverType is compatible with the given Event type.
  static constexpr bool observerSupportsEvent = std::is_base_of_v<ObserverFor<Event>, ObserverType>;

  // Internal helper methods to allow parameter pack expansion to work with the attach/detach methods below

  template <typename Event, typename ObserverType>
  void attachForEvent(ObserverType* observer) {
    if constexpr (observerSupportsEvent<Event, ObserverType>) {
      SubjectFor<Event>::attach(static_cast<ObserverFor<Event>*>(observer));
    }
  }

  template <typename Event, typename ObserverType>
  void detachForEvent(ObserverType* observer) {
    if constexpr (observerSupportsEvent<Event, ObserverType>) {
      SubjectFor<Event>::detach(static_cast<ObserverFor<Event>*>(observer));
    }
  }

  // Pull all notify methods out from the base classes to avoid unqualified name lookup causing compiler errors
  // when multiple base classes share the same method names (even if those methods would be unambiguous during actual overload resolution).
 protected:
  using SubjectFor<Events>::notify...;

 public:
  virtual ~Subject() = default;

  // Attach a (compatibly-typed) observer to this Subject, which will be notified upon a corresponding event being emitted.
  // The Subject does not take ownership of the observer; it is the caller's responsibility to ensure the Observer
  // lives longer than the duration of its attachment.
  //
  // This, by default, subscribes the observer to all event types that it is compatible with.
  // Consumers that want to subscribe to specific events can use the attach/detach methods from SubjectFor instead.
  template <typename ObserverType>
    requires(std::is_class_v<ObserverType>) &&
            // non-empty intersection (at least 1 matches)
            (std::is_base_of_v<ObserverFor<Events>, ObserverType> || ...)
  void attach(ObserverType* observer) {
    (attachForEvent<Events>(observer), ...);
  }

  // Detach an observer from this Subject, which will no longer be notified upon a corresponding event being emitted.
  //
  // This, by default, unsubscribes the observer from all event types that it is compatible with.
  // Consumers that want to unsubscribe from specific events can use the attach/detach methods from SubjectFor instead.
  template <typename ObserverType>
    requires(std::is_class_v<ObserverType>) &&
            // non-empty intersection (at least 1 matches)
            (std::is_base_of_v<ObserverFor<Events>, ObserverType> || ...)
  void detach(ObserverType* observer) {
    (detachForEvent<Events>(observer), ...);
  }
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
