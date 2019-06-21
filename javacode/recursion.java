import java.util.Scanner;

public class recursion{
    public static int divide(int num1, int num2){
        int tmp;
        if (num1 < num2){
            tmp = num1;
            num1 = num2;
            num2 = tmp;
        }
        if (num2 == 0)
            return num1;
        return divide(num1 - num2, num2);
    }
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        int n1 = sc.nextInt();
        int n2 = sc.nextInt();
        int res = divide(n1, n2);
        System.out.println(res);
    }
}

