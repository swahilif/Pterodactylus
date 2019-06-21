

public class array{
    public static void main(String[] args){
        /* Show 2: Polym */
        //father a = new father();
        int [][]a = new int[3][5];
        
        a[1][3] = 4;
        a[2][1] = 5;
        int []b = new int[5];
        for(int i = 0; i < 5; i ++)
            b[i] = i;
        a[0] = b;
        for(int i = 0; i < 3; i ++)
            for(int j = 0; j < 5; j ++)
                System.out.println(a[i][j]);
        int []tmp = new int[30];
    }
}
