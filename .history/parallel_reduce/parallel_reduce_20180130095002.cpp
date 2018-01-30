#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "../ocl/ocl_loader.hpp"

#define VECSIZE 1000000
#define WGSIZE 512
#define NUMWORKGROUPS (VECSIZE / WGSIZE + 1)

int get_global_id(int i ){
	return i;
}
void barrier(int i){
	return;
}
void twoStagePccl(char **image, int *changed, int width, int height)
{

    int row = get_global_id(1);
    int col = get_global_id(0);

    if ((row < height) && (col < width) && image[row][col] == 1)
    {

        char minLabel = row * width + col;
        image[row][col] = minLabel;
        barrier(1);

        int nrow = row;
        int ncol = col;
        char nlabel = 0;
        int i=-1, j=-1;
        for (i = -1; i < 2; i++){
            for (j = -1; j < 2; j++)
            {
                nrow = row + i;
                ncol = col + j;
                nlabel = image[nrow][ncol];
                if ((nrow > 0) && (nrow < height) && (ncol > 0) && (ncol < width) && (nlabel < minLabel))
                {
                    minLabel = nlabel;
                }
            }
        }
    }
}

int main()
{
	OpenClLoader ocl;

	Kernel *kernel = ocl.load_kernel("psr.cl", "parallelSum");

	float vec[VECSIZE];
	float result[NUMWORKGROUPS];

	for(int i =0; i < VECSIZE; i++)
		vec[i] = 1;


	size_t globalWorkSize = VECSIZE;
	size_t localWorkSize = WGSIZE;

	kernel->setBufferArg(0, VECSIZE * sizeof(float), vec);
	kernel->setBufferArg(1, NUMWORKGROUPS * sizeof(float), result);
	kernel->setLocalArg(2, NUMWORKGROUPS * sizeof(float));

	kernel->execute("ParallelReduction", 1, &globalWorkSize, &localWorkSize);

	kernel->getResult(1, NUMWORKGROUPS * sizeof(float), result);

	
	float sum = 0;
	for (int i = 0; i < NUMWORKGROUPS; i++){
		sum += result[i];
	}

	float sum2 = 0;
	for(int i =0;i<VECSIZE;i++){
		sum2 += vec[i];
	}
	
	printf("%f %f", sum, sum2);
}

