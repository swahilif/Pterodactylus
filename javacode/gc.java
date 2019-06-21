import java.util.Scanner;
class father{
    public father attr;
    public int num;

    public void test_gc(){
        father tmp = new father();
        father s = new father();
        s.attr = tmp;
        tmp.attr = s;
    }
}

public class gc{
    public int att;
    public static void main(String[] args){
        father a = new father();
        a.test_gc();
        father c = new father();
        a.attr = c;
        c = new father();
        father e = new father();
    }
}


