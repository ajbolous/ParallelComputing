__kernel void parallelSum(__global double *vecArray, __global double *globalSums, __local double *localSums)
{
    int globalId = get_global_id(0);
    int groupId = get_group_id(0);
    int localId = get_local_id(0);
    int localSize = get_local_size(0);

    // Copy from global to local memory
    localSums[localId] = vecArray[globalId];

    // Loop for computing localSums : divide WorkGroup into 2 parts
    for (int i = localSize / 2; i > 0; i /= 2)
    {
        // Waiting for each 2x2 addition into given workgroup
        barrier(CLK_LOCAL_MEM_FENCE);
        // Add elements 2 by 2 between local_id and local_id + stride
        if (localId < i)
            localSums[localId] += localSums[localId + i];
    }
    // Write result into partialSums[nWorkGroups]

    if (localId == 0)
    {
        globalSums[groupId] = localSums[0];
        printf("globalSums: %d,  %lf \n", groupId, globalSums[groupId]);
    }
}
