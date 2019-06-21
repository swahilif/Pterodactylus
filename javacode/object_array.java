class A{
    public int m;
    public int n;
}


public class object_array{
    public static void main(String[] args){
        /* Show 2: Polym */
        //father a = new father();
        A [][]a = new A[3][10];
        a[1][2].m = 200;
        A []b = new A[10];
        for(int i = 0; i < 10; i ++)
            b[i].m = i;
        System.out.println(b[3].m);
        a[0] = b;
        for(int i = 0; i < 3; i ++)
            for(int j = 0; j < 10; j ++)
                    System.out.println(a[i][j].m);
        int []tmp = new int[30];
    }
}
