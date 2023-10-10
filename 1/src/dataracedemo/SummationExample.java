package dataracedemo;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

class SummationResult {
    int value = 0;
}

class Summer extends Thread {
    private final SummationResult result;
    private final List<Integer> data;
    private final int startPos;
    private final int length;

    Summer(SummationResult result, List<Integer> data, int startPos, int length) {
        this.result = result;
        this.data = data;
        this.startPos = startPos;
        this.length = length;
    }

    @Override
    public void run() {
        int partialSum = 0;
        for (int i = 0; i < length; i++) {
            partialSum += data.get(startPos + i);
        }
        // Data race! This is shorthand for:
        //   result.value = result.value + partialSum;
        // It is possible for two threads to read the same "result.value", and then to write
        // updated values to this field, so that some updates get lost.
        result.value += partialSum;
    }
}

public class SummationExample {
    public static final int NUM_ELEMENTS = 1 << 24;
    public static final int NUM_THREADS = 1 << 12;

    public static final int ELEMENTS_PER_THREAD = NUM_ELEMENTS / NUM_THREADS;

    public static void main(String[] args) {
        List<Integer> data = new ArrayList<>();
        for (int i = 0; i < NUM_ELEMENTS; i++) {
            data.add(1);
        }
        SummationResult result = new SummationResult();

        List<Thread> threads = new LinkedList<>();
        for (int i = 0; i < NUM_THREADS; i++) {
            Thread t = new Summer(result, data, i * ELEMENTS_PER_THREAD, ELEMENTS_PER_THREAD);
            t.start();
            threads.add(t);
        }

        try {
            for (Thread t : threads) {
                t.join();
            }
        } catch (InterruptedException exception) {
            System.err.println("Something went wrong!");
            System.exit(1);
        }

        System.out.println(result.value);

    }

}
