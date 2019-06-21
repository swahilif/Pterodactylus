import java.util.Scanner;
public class fib {
    public static void main(String[] argv) {
        Scanner sc = new Scanner(System.in);
        int k = sc.nextInt();
        int a = 1, b = 1;
        for (int i = 1; i <= k; i++) {
            int t = a + b;
            b = a;
            a = t; 
        }
        System.out.println(a);
    }
}
