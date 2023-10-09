package deadlockdemo;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;

public class ThreadSafeList<E> {

  private final List<E> elements = new LinkedList<>();

  private final StringBuffer log = new StringBuffer();

  public Optional<E> pop() {
    // TODO: populate
    throw new RuntimeException();
  }

  public void push(E element) {
    // TODO: populate
    throw new RuntimeException();
  }

  public String getLog() {
    return log.toString();
  }

}
