__kernel void twoStagePccl(__global uchar *image, __global int *changed, int width, int height)
{

    int row = get_global_id(1);
    int col = get_global_id(0);
    int index = row * width + col;

    if ((row < height) && (col < width) && image[index] == 1)
    {

        uchar minLabel = row * width + col;
        image[index] = minLabel;
        barrier(CLK_LOCAL_MEM_FENCE);

        int nrow = row;
        int ncol = col;
        int nindex = nrow * width + ncol;
        uchar nlabel = 0;

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                nrow = row + i;
                ncol = col + j;
                nindex = nrow * width + ncol;
                nlabel = image[nindex];
                if ((nrow > 0) && (nrow < height) && (ncol > 0) && (ncol < width) && (nlabel < minLabel))
                {
                    minLabel = nlabel;
                    (*changed)++;
                }
            }
        }
    }
}