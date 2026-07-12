export module observer;

#ifdef __INTELLISENSE__
#include <list>
#include <type_traits>
#include <typeinfo>
#else
import <type_traits>;
import <typeinfo>;
import <list>;
#endif

export template <typename Event>
  requires std::is_class_v<Event>
class Subject;

// Interface representing an observer that tracks a single event type.
template <typename Event>
  requires std::is_class_v<Event>
class Observer {
  friend class Subject<Event>;
  // Trigger effects upon an event of the given type being emitted.
  virtual void onNotify(const Event& event) = 0;

 public:
  virtual ~Observer() = default;
};

template <typename... Events>
  requires(std::is_class_v<Events> && ...)
class MultiObserver : public Observer<Events>... {};

// Class representing a subject that can be observed for 1 or more event types.
export template <typename Event>
  requires std::is_class_v<Event>
class Subject {
  // The backing list of observers.
  std::list<Observer<Event>*> observers;

 protected:
  // Notify all attached observers of a corresponding event being emitted.
  void notify(const Event& event) final;

 public:
  // Attach a (compatibly-typed) observer to this Subject, which will be notified upon a corresponding event being emitted.
  //
  // The Subject does not take ownership of the observer; it is the caller's responsibility to ensure the Observer
  // lives longer than the duration of its attachment.
  void attach(Observer<Event>* observer) final;
  // Detach an observer from this Subject, which will no longer be notified upon a corresponding event being emitted.
  void detach(Observer<Event>* observer) final;

  virtual ~Subject() = default;
};
