class father{
	public static void rap(){
		System.out.println(3);
	}
}

class daughter extends father{
	public static void rap(){
		System.out.println(5);
	}
}

public class polym_static{
	public static void main(String[] args){
		/* Show 2: Polym */
		father s = new daughter();
		s.rap();
	}
}

