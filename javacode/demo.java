class jam {
    public boolean check(int c) {
        if (c==0) return true;
        return false;
    }
    public int seek() {
        return 19;
    }
}

public class demo extends jam {
    public int a;
    public int b;
    public int e;
    public void read() {
        b = 4; 
        e = 9;
    }
    public boolean check(int c) {
        a=c-1;
        b=c+3;
        if (a+b > c) return true;
        return false;
    }
}
