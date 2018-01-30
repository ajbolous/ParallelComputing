__kernel void twoStagePccl(__global uchar* image,  __global int *changed, int width, int height)
{

    int row = get_global_id(1);
    int col = get_global_id(0);
    int index = row * width + col;

    if ((row < height) && (col < width) && image[index] != 0)
    {
        int minLabel = image[index];

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
                uchar nlabel = image[nindex];
                if ((nrow > 0) && (nrow < height) && (ncol > 0) && (ncol < width) && (nlabel > 0) && (nlabel < minLabel))
                {
                    minLabel = nlabel;
                    (*changed)++;
                }
            }
        }
        image[index] = minLabel;
    }
}

__kernel void firstRunPccl(__global uchar *image, int width, int height)
{
    int row = get_global_id(1);
    int col = get_global_id(0);
    int index = row * width + col;
    if ((row < height) && (col < width) && image[index] > 0){
        image[index] = (index / 8150 + 1) * 5;
    }
}