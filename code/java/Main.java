/*
Student:Christian Lockley
School:University of North Florida Spring 2020
Class:COP 4610 – Operating Systems
Professor:Dr. Sanjay Ahuja
*/

import java.util.Arrays;

public class Main {
    static class Buffer {
        static public int EMPTY = -1;
        static public int FULL = 0;
        static final public int BUFFER_SIZE = 96;
        private int freeSlots = BUFFER_SIZE;
        private int[] a = new int[BUFFER_SIZE];
        Buffer() {
            Arrays.fill(a, EMPTY);
        }
        public synchronized int addItem() {
            while (freeSlots == 0) {
                System.out.println("Buffer full waiting...");
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            int i;
            for (i = 0; i < BUFFER_SIZE; ++i) {
                if (a[i] == EMPTY) {
                    a[i] = FULL;
                    notify();
                    break;
                }
            }
            --freeSlots;
            return i;
        }

        public synchronized int removeItem() {
            notify();
            while (freeSlots == BUFFER_SIZE) {
                System.out.println("Buffer empty waiting...");
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            int i;
            for (i = 0; i < BUFFER_SIZE; ++i) {
                if (a[i] == FULL) {
                    a[i] = EMPTY;
                    break;
                }
            }
            ++freeSlots;
            return i;
        }
    }

    public static void main(String[] args) {

        Buffer buf = new Buffer();
        Runnable Producer = () -> {
            while (true) {
                System.out.printf("Adding item to buffer at index %d\n", buf.addItem());
            }
        };
        Runnable Consumer = () -> {
            while (true) {
                System.out.printf("Removing item to buffer at index %d\n", buf.removeItem());
            }
        };
        if (args.length == 1) {
            if (args[0].compareTo("case1") == 0) {
                Thread ProducerThread = new Thread(Producer);
                Thread ConsumerThread = new Thread(Consumer);
                ProducerThread.start();
                ConsumerThread.start();
            } else if (args[0].compareTo("case2") == 0) {
                Thread ProducerThread = new Thread(Producer);
                Thread ProducerThread2 = new Thread(Producer);
                Thread ConsumerThread = new Thread(Consumer);
                ProducerThread.start();
                ProducerThread2.start();
                ConsumerThread.start();
            } else if (args[0].compareTo("case3") == 0) {
                Thread ProducerThread = new Thread(Producer);
                ProducerThread.start();
            } else if (args[0].compareTo("case4") == 0) {
                Thread ConsumerThread = new Thread(Producer);
                ConsumerThread.start();
            }
        } else {
            Thread ProducerThread = new Thread(Producer);
            Thread ConsumerThread = new Thread(Consumer);
            ProducerThread.start();
            ConsumerThread.start();
        }
    }
}
