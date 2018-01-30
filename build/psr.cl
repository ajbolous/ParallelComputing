__kernel void parallelSum(__global float *vecArray, __global float *totalSums, __local float *workGroupSums, int size)
{
    int globalId = get_global_id(0);

    int groupId = get_group_id(0);
    int localId = get_local_id(0);

    workGroupSums[localId] = globalId >= size ? 0 : vecArray[globalId];

    for (int i = get_local_size(0) / 2; i > 0; i /= 2)
    {
        barrier(CLK_LOCAL_MEM_FENCE);
        if (localId < i)
            workGroupSums[localId] += workGroupSums[localId + i];
    }

    if (localId == 0)
        totalSums[groupId] = workGroupSums[0];
}