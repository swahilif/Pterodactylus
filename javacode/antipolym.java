class father{
	public void print(){
		System.out.println(1);
	}
}

class daughter extends father{
}

public class antipolym{
	public static void main(String[] args){
		/* Show 2: Polym */
		daughter s = new daughter();
		s.print();
	}
}

