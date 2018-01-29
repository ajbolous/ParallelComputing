__kernel void mul2image(float* imageIn, float* imageOut, int width, int height){

        int row = get_group_id(1)*get_local_size(1) + get_local_id(1);
        int col = get_group_id(0)*get_local_size(0) + get_local_id(0);

        if((row < height) && (col < width)){
            imageOut[Row*width + height]
        } 

?
