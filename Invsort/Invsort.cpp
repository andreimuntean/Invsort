#include <algorithm>
#include <bitset>
#include <fstream>
#include <vector>

struct Swap
{
    short i = -1;
    short j = -1;
};

struct Pair
{
    std::bitset<1> binaryValue;
    short sequenceValue;
};

short sequenceSize;
short sequence[32000];
short sortedSequence[32000];
Pair* binarySequence;
std::vector<Swap> swaps;

void read(std::string path)
{
    std::ifstream file(path);

    file >> sequenceSize;

    for (short sequenceIndex = 0; sequenceIndex < sequenceSize; ++sequenceIndex)
    {
        file >> sequence[sequenceIndex];
    }

    file.close();
}

void initialize()
{
    binarySequence = new Pair[sequenceSize];
    swaps.reserve(4000000);
    std::copy(sequence, sequence + sequenceSize, sortedSequence);
    std::sort(sortedSequence, sortedSequence + sequenceSize);
}

// Both indexes are inclusive.
void bitInvsort(short startIndex, short endIndex)
{
    if (startIndex < endIndex)
    {
        short medianIndex = ((int)startIndex + (int)endIndex) / 2;
        Swap swap;

        bitInvsort(startIndex, medianIndex);
        bitInvsort(medianIndex + 1, endIndex);

        // Gets the first occurence of 1 from the first partition.
        for (short index = startIndex; index <= medianIndex; ++index)
        {
            if (binarySequence[index].binaryValue == 1)
            {
                swap.i = index;
                break;
            }
        }

        // Stops if the value 1 was not found.
        if (swap.i == -1)
        {
            return;
        }

        // Gets the last occurrence of 0 from the second partition.
        for (short index = endIndex; index > medianIndex; --index)
        {
            if (binarySequence[index].binaryValue == 0)
            {
                swap.j = index;
                break;
            }
        }

        // Stops if the value 0 was not found.
        if (swap.j == -1)
        {
            return;
        }

        swaps.push_back(swap);
        std::reverse(binarySequence + swap.i, binarySequence + swap.j + 1);
    }
}

// Both indexes are inclusive.
void invsort(short startIndex, short endIndex)
{
    if (startIndex < endIndex)
    {
        short medianIndex = ((int)startIndex + (int)endIndex) / 2;
        short lowest = sortedSequence[startIndex];
        short median = sortedSequence[medianIndex];
        short highest = sortedSequence[endIndex];

        // Continues only if this partition is not sorted.
        if (lowest < median || median < highest)
        {
            short zeroPartitionLength = 0;

            // Prevents stack overflow exceptions by evading
            // situations where all values become 1.
            if (lowest == median)
            {
                ++median;
            }

            // Turns all values smaller than "median" to 0 and all values
            // greater than it to 1.
            for (short index = startIndex; index <= endIndex; ++index)
            {
                if (sequence[index] < median)
                {
                    binarySequence[index].binaryValue = 0;
                    ++zeroPartitionLength;
                }
                else
                {
                    binarySequence[index].binaryValue = 1;
                }

                binarySequence[index].sequenceValue = sequence[index];
            }

            // Sorts the binary numbers.
            bitInvsort(startIndex, endIndex);

            // Places all values smaller than median before it and all values
            // greater than median after it.
            for (short index = startIndex; index <= endIndex; ++index)
            {
                sequence[index] = binarySequence[index].sequenceValue;
            }

            invsort(startIndex, startIndex + zeroPartitionLength - 1);
            invsort(startIndex + zeroPartitionLength, endIndex);
        }
    }
}

void write(std::string path)
{
    std::ofstream file(path);

    for (int swapIndex = 0; swapIndex < swaps.size(); ++swapIndex)
    {
        file << swaps[swapIndex].i + 1 << ' ' << swaps[swapIndex].j + 1 << '\n';
    }
}

int main(int argc, char** argv)
{
    read(argc == 2 || argc == 3 ? argv[1] : "invsort.in");
    initialize();
    invsort(0, sequenceSize - 1);
    write(argc == 3 ? argv[2] : "invsort.out");

    return 0;
}