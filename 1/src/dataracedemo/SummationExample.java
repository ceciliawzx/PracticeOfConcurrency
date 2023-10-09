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
        // TODO: implement
        throw new RuntimeException();
    }
}

public class SummationExample {
    public static final int NUM_ELEMENTS = 1 << 24;
    public static final int NUM_THREADS = 1 << 2;

    public static final int ELEMENTS_PER_THREAD = NUM_ELEMENTS / NUM_THREADS;

    public static void main(String[] args) {
        List<Integer> data = new ArrayList<>();
        for (int i = 0; i < NUM_ELEMENTS; i++) {
            data.add(1);
        }
        SummationResult result = new SummationResult();

        // TODO: launch some summer threads and join them.

        System.out.println(result.value);

    }

}
