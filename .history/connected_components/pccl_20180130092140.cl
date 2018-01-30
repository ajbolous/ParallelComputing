__kernel void twoStagePccl(char **imageIn, int *changed, int width, int height)
{

    int row = get_global_id(1);
    int col = get_global_id(0);

    if ((row < height) && (col < width) && imageIn[row][col] == 1)
    {

        char minLabel = row * width + col;
        imageIn[row][col] = minLabel;
        barrier(CLK_LOCAL_MEM_FENCE);

        int nrow = row;
        int ncol = col;
        char nlabel = 0;

        for (int i = -1; i < 2; i++)
            for (int j = -1; j < 2; j++)
            {
                nrow = row + i;
                ncol = col + j;
                nlabel = image[nrow][ncol];
                if (nrow > 0 && nrow < height && ncol > 0 && ncol < width,  nlabel < minLabel){
                    minLabel = nlabel;
                    *changed++;
                }
            }
    }
}