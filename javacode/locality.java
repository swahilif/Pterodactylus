class test{
	public int num;
	public void func (int i, int j, int k){
		num = i * j - k;
	}

}

public class locality{

	public static void main(String[] args){
		test[][] array = new test[1000][1000];
		int[] b;
		for (int i = 0; i < 1000; i ++){
			for( int j = 0; j < 1000; j ++){
				array[i][j] = new test();
				b = new int[5];
			}
		}
		int s;
		for (int k = 0; k < 100; k ++)
			for(int i = 0; i < 1000; i ++)
				for(int j = 0; j < 1000; j ++)
					array[i][j].func(i, j, k);
					//s = 1;
					//array[i][j].num = i + j - k;
	}
}
