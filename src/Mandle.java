public class Mandle {
    static float iterates = 1000.0f;
    static double scale = 0.001;
    static double rOff = -0.75, iOff =0.1; 

    static double cR = 0.0;
    static double cI = 0.0;
    static double zR = 0.0;
    static double zI = 0.0;
    static double temp = 0.0;
    static float index = 0.0f;

    static float getDivergeAt(float r,float i){
        cR = (r * scale) + rOff;
        cI = (i * scale) + iOff;

        zR = 0.0;
        zI = 0.0;

        temp = 0;
        index = 0;
        while((index < iterates) && ((zR * zR + zI * zI) < 4.0)){
            temp = zR * zR - zI * zI + cR;
            zI = 2.0 * zR * zI + cI;
            zR = temp;
            index++;
        }
        return index/iterates;
    }

    public static void main(String[] args){
        if(args.length < 1){
            System.err.println("no width arg");
            return;
        }
        if(args.length < 2){
            System.err.println("no height arg");
            return;
        }
        int width = Integer.parseInt(args[0]);
        int height = Integer.parseInt(args[1]);
        System.out.println(String.format("Width: %d Height: %d", width, height));

        float[] arr = new float[width * height];

        for(int j=0; j < height; j++){
            for(int i=0; i < width; i++){
                arr[i + j*width] = getDivergeAt(
                    (float)(i-(width/2)),
                    (float)(j-(width/2))
                );
            }
        }
        System.out.println(arr[(int)(Math.random() * arr.length)]);
        return;
    }
}