#include <cstdlib>
#include <iostream>

#include <caffe/blob.hpp>

int main()
{
    caffe::Blob<float> blob(3, 4, 1, 1);
    return EXIT_SUCCESS;
}
