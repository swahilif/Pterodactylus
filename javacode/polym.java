class father{
	public int num;
	public void changeNum(){
		num = 20;
	}
}

class daughter extends father{
	public void changeNum(){
		num = 40;
	}
}

public class polym{
	public static void main(String[] args){

		/* Show 2: Polym */
		father s = new daughter();
		s.changeNum();
		System.out.println(s.num);
	}
}

