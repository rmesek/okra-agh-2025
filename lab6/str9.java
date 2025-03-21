import java.util.Scanner;

public class str9 {
    private static final int REPEATS = 50000;

    /**
     * Uses System.nanoTime() for high-precision timing
     */
    private static double dclock() {
        return System.nanoTime() / 1.0e9; // Convert nanoseconds to seconds
    }

    /**
     * Remove control characters (ASCII < 0x20) from a string
     */
    private static String removeCtrl(String s) {
        StringBuilder result = new StringBuilder(s.length());
        
        for (int begin = 0, i = begin, end = s.length(); begin < end; begin = i + 1) {
            for (i = begin; i < end; i++) {
                if (s.charAt(i) < 0x20) break;
            }
            result.append(s.substring(begin, i));
        }
        
        return result.toString();
    }

    public static void main(String[] args) {
        System.out.println("call to remove");
        
        StringBuilder s = new StringBuilder();
        String result = "";
        
        // Read input from stdin
        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNextLine()) {
            s.append(scanner.nextLine()).append("\n");
        }
        scanner.close();
        
        String input = s.toString();
        
        // Time the removeCtrl function
        double dtime = dclock();
        for (int i = 0; i < REPEATS; i++) {
            result = removeCtrl(input);
        }
        dtime = dclock() - dtime;
        
        System.out.println(result);
        System.out.println("Time: " + dtime);
        System.out.flush();
    }
}