#include <iostream>
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>

#include <sys/time.h>

#include <opencv2/opencv.hpp>

std::mutex mtex;

void product(const std::vector<int> &v1, const std::vector<int> &v2, int start, int end, double *sum)
{
    *sum = 0;
    for (int i = start; i <= end; i++)
        (*sum) += v1[i] * v2[i];
}

void product2(const std::vector<int> &v1, const std::vector<int> &v2, int start, int end, double &sum)
{
    double val = 0;
    for (int i = start; i <= end; i++)
        val += v1[i] * v2[i];

    std::lock_guard<std::mutex> lock(mtex);
    sum += val;
}

std::vector<int> bounds(int length, int numParts)
{
    std::vector<int> parts;
    int partSize = length/numParts;

    parts.push_back(0);
    if (length <= numParts || numParts == 1)
    {
        parts.push_back(length - 1);
        return parts;
    }

    // align partSize
    int mod = partSize % 8;
    if (mod)
        partSize += (8 - mod);

    parts.push_back(partSize - 1);
    int start = 0, end = 0;

    for (int i = 1; i < numParts-1; i++)
    {
        start = parts[2*(i-1) + 1] + 1;
        end = start + partSize - 1;

        parts.push_back(start);
        parts.push_back(end);
    }
    start = parts[2 *(numParts-2) + 1] + 1;
    end = length - 1;

    parts.push_back(start);
    parts.push_back(end);

    return parts;
}

int main()
{
    int n = 123456789;
    int numThreads = 8;

    std::vector<int> v1(n, 1);
    std::vector<int> v2(n, 2);

    if (n <= 256)
        numThreads = 1;

    std::vector<int> parts = bounds(n, numThreads);
    //for (uint i = 0; i < parts.size(); i += 2)
    //    std::cout << "Part " << (i/2) << " (" << parts[i] << " --> " << parts[i+1] << ")\n";

    //return 0;

    timeval t1;
    gettimeofday(&t1, NULL);

    std::vector<std::thread> threads(numThreads);

    int padding = 16;
    int spacing = 16;
    int numSums = padding + numThreads * spacing;
    std::vector<double> sums(numSums, 0);

    for (int i = 1; i < numThreads; i++)
    {
        threads[i] = std::thread(product, std::cref(v1), std::cref(v2), parts[2*i], parts[2*i+1], &sums[padding + i*spacing]);
        //threads[i] = std::thread(product, v1, v2, parts[2*i], parts[2*i+1], &sums[padding + i*spacing]);
    }

    product(v1, v2, parts[0], parts[1], &sums[padding]);

    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    double total = 0;
    for (int i = padding; i < numSums; i += spacing)
    {
        total += sums[i];
        std::cout << "sum = " << sums[i] << "\n";
    }

    std::cout << "Total = " << total << "\n";

    timeval t2;
    gettimeofday(&t2, NULL);

    std::cout << "Using " << numThreads << " threads, running time = " << ((t2.tv_sec - t1.tv_sec)*1000 + (t2.tv_usec - t1.tv_usec)/1000.) << " ms\n";

    return 0;
}
