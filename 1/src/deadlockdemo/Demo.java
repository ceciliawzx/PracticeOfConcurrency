package deadlockdemo;

class Pusher extends Thread {
  private final ThreadSafeList<Integer> list;
  private final int numOperations;
  Pusher(ThreadSafeList<Integer> list, int numOperations) {
    this.list = list;
    this.numOperations = numOperations;
  }

  @Override
  public void run() {
    for (int i = 0; i < numOperations; i++) {
      list.push(i);
    }
  }
}

class Popper extends Thread {
  private final ThreadSafeList<Integer> list;
  private final int numOperations;
  Popper(ThreadSafeList<Integer> list, int numOperations) {
    this.list = list;
    this.numOperations = numOperations;
  }
  @Override
  public void run() {
    for (int i = 0; i < numOperations; i++) {
      list.pop();
    }
  }
}

public class Demo {

  public static void main(String[] args) {
    ThreadSafeList<Integer> list = new ThreadSafeList<>();
    final int numOperations = 5;
    Thread pusher = new Pusher(list, numOperations);
    Thread popper = new Popper(list, numOperations);
    pusher.start();
    popper.start();
    try {
      pusher.join();
      popper.join();
    } catch (InterruptedException exception) {
      System.err.println("Something went wrong!");
      System.exit(1);
    }
    System.out.println(list.getLog());
  }

}
