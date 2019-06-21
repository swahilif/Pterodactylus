public class test1{
	public static int cal(int p1, int p2, int p3){
		//return cal2(p1 + p2, p3) / p2 - p2;
		return p1 * p1 * p3 / p2 - p2;
	}
	public static int cal2(int p1, int p2){
		return 2 * p1 * p2;
	}
	public static int jntm(){
		return 1;
	}
	public static void main(String[] args){
		/* Show 1: Calculation and Calling of any recursion */
		int c = cal(1, 2, 3);
		System.out.println(c); 
	}
}

