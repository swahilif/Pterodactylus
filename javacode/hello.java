import java.util.Scanner;
class father{
	public father attr;
	public int num;
	public static void rap(){
		System.out.println(3);
	}
	public void dance(){
		System.out.println(1);
	}
	public void sing(){
		num = 1;
	}
	public int divide(int num1, int num2){
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
	public void test(){
		father tmp = new father();
		father s = new father();
		s.attr = tmp;
		tmp.attr = s;
		father pp;
		for (int i = 1; i < 10; i ++)
			pp = new father();
	}
}

class daughter extends father{
	public static void rap(){
		System.out.println(5);
	}
	public void sing(){
		num = 0;
	}
}

public class hello{
	public static int att = 10;
	public hello (int s){
		att = s;
	}
	public static void main(String[] args){
		/* Show 2: Polym */
		//father a = new father();
		father []a = new father[5];
        a[4] = new father();
        a[4].num = 1;

		Scanner sc = new Scanner(System.in);
		int n1 = sc.nextInt();
		int n2 = sc.nextInt();
		//System.out.println(age);
		father b = new father();
		//b.test();
		int res = b.divide(n1, n2);
		System.out.println(res);
		//b = a;
		/*
		father c = new father();
		b.attr = c;
		c = new father();
		father e = new father();
		*/
	}
}

