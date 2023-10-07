import java.util.concurrent.*;
import java.util.Arrays;

public class MergeSortTask extends RecursiveAction {

    static final int SIZE = 100;
    static final int THRESHOLD = 10;

    private int begin;
    private int end;
    private int[] array;

    public MergeSortTask(int begin, int end, int[] array) {
        this.begin = begin;
        this.end = end;
        this.array = array;
    }

    protected void compute() {
        if (end - begin < THRESHOLD)
            insertionSort(begin, end, array);  
        else {
            int mid = begin + (end - begin) / 2;

            MergeSortTask leftTask = new MergeSortTask(begin, mid, array);
            MergeSortTask rightTask = new MergeSortTask(mid + 1, end, array);
            
            invokeAll(leftTask, rightTask); 
            
        }
    }

    void insertionSort(int begin, int end, int array[]) {
        int size = end - begin;
        System.out.println("begin: " + begin + " end: " + end); 
        for (int step = begin + 1; step < size; step++) {
            int key = array[step];
            int j = step - 1;

            //Compare key with each elm on the left of it until an elm smaller than it is found
            //For descending order, change key<array[j] to key>array[j].
            while (j >= begin && key < array[j]) {
                array[j + 1] = array[j];
                --j;
            }
            array[j + 1] = key;
        }
    }
    
    public static void main(String[] args) {
        ForkJoinPool pool = new ForkJoinPool();
        int[] array = new int[SIZE];

        java.util.Random rand = new java.util.Random();

        System.out.println("----------------------Unsorted--------------------");
        for (int i = 0; i < SIZE; i++){
            array[i] = rand.nextInt(10);
            System.out.println(array[i]);
        }

        MergeSortTask task = new MergeSortTask(0, SIZE-1, array);
        pool.invoke(task);

        System.out.println("-----------------------Sorted---------------------");
        System.out.println(Arrays.toString(array));
    

    }

}
