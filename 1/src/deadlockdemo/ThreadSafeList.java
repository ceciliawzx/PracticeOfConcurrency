package deadlockdemo;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;

public class ThreadSafeList<E> {

  private final List<E> elements = new LinkedList<>();

  private final StringBuffer log = new StringBuffer();

  public Optional<E> pop() {
    // Note the locking order: "elements", then "log". And see below ...
    synchronized (elements) {
      synchronized (log) {
        log.append("Entered pop\n");
        if (elements.isEmpty()) {
          log.append("pop failed\n");
          return Optional.empty();
        }
        log.append("pop succeeded\n");
        return Optional.of(elements.remove(0));
      }
    }
  }

  public void push(E element) {
    // ... here we have a different locking order: "log", then "elements".
    // This can lead to the pusher thread holding a lock on "log", the popper thread
    // holding a lock on "elements", and it being impossible for each thread to get the
    // lock that the other holds. Deadlock!
    synchronized (log) {
      synchronized (elements) {
        log.append("Entered push\n");
        elements.add(element);
      }
    }
  }

  public String getLog() {
    return log.toString();
  }

}
