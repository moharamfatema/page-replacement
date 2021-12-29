#include <vector>
#include <string>
#include <algorithm>
#include <bits/stdc++.h>
#include <iostream>


typedef std::vector<int>::iterator iterator;
typedef std::vector<std::vector<int>> Trace;

class PageReplacement
{
    int noofFrames;
    std::string algo;
    std::vector<int> sequence;
    std::vector<int> frames;
    Trace trace;
    std::vector<bool> pageFaults;
    int noOfPageFaults;

    bool pageFault(iterator it)
    {
        /*search for page in frames*/
        if (*std::find(frames.begin(), frames.end(), *it) == *it)
        {
            /*page found*/
            pageFaults.push_back(false);
            return false;
        }
        else if (frames.size() < noofFrames)
        {
            /*frames are not full yet*/
            frames.push_back(*it);
            pageFaults.push_back(false);
            return false;
        }
        else
            return true;
    }

public:
    PageReplacement(
        const int noofFrames,
        const std::string algo,
        const std::vector<int> sequence)
        : noofFrames(noofFrames),
          algo(algo),
          sequence(sequence)
    {
        frames.reserve(noofFrames);
        noOfPageFaults = 0;
    }

    std::vector<std::vector<int>> go()
    {
        if (algo == "FIFO")
            return fifo();
        else if (algo == "CLOCK")
            return clock();
        else if (algo == "LRU")
            return lru();
        else
            return optimal();
    }

    /*to place in private after development*/
    iterator leastRecentlyUsed(int i)
    {
        std::priority_queue<std::pair<int, iterator>> q;
        auto framesit = frames.begin();
        std::reverse_iterator<iterator> found;
        int index;
        for (framesit = frames.begin(); framesit < frames.end(); framesit++)
        {
            found = std::find(sequence.rend() - i - 1, sequence.rend(), *framesit);
            index = found - sequence.rend() + i + 1;
            q.push(std::make_pair(index, framesit));
        }
        return q.top().second;
    }

    iterator predict(iterator it)
    {
        std::priority_queue<std::pair<int, iterator>> q;
        auto framesit = frames.begin();
        iterator found;
        int index;
        for (framesit = frames.begin(); framesit < frames.end(); framesit++)
        {
            found = std::find(it, sequence.end(), *framesit);
            if (found == sequence.end())
            {
                return framesit;
            }
            index = found - it;
            q.push(std::make_pair(index, framesit));
        }
        return q.top().second;
    }

    Trace optimal()
    {
        noOfPageFaults = 0;
        for (auto i = sequence.begin(); i < sequence.end(); i++)
        {
            if (pageFault(i))
            {
                /*page fault*/
                pageFaults.push_back(true);
                noOfPageFaults++;
                *predict(i) = *i;
            }
            /*record frames in trace*/
            trace.push_back(frames);
        }
        return trace;
    }

    Trace fifo()
    {
        auto it = frames.begin();
        noOfPageFaults = 0;
        for (auto i = sequence.begin(); i < sequence.end(); i++)
        {
            if (pageFault(i))
            {
                /*page fault*/
                pageFaults.push_back(true);
                noOfPageFaults++;
                *it = *i;
                if (it + 1 == frames.end())
                    it = frames.begin();
                else
                    it++;
            }
            /*record frames in trace*/
            trace.push_back(frames);
        }
        return trace;
    }

    Trace lru()
    {
        noOfPageFaults = 0;
        int index;
        for (auto i = sequence.begin(); i < sequence.end(); i++)
        {
            if (pageFault(i))
            {
                /*page fault*/
                pageFaults.push_back(true);
                noOfPageFaults++;
                index = i - sequence.begin();
                *leastRecentlyUsed(index) = *i;
            }
            /*record frames in trace*/
            trace.push_back(frames);
        }
        return trace;
    }

    Trace clock()
    {
        std::vector<bool> arr;

        for (int j = 0; j < noofFrames; j++)
        {
            arr.push_back(false); // no stars
        }

        auto it = frames.begin();
        noOfPageFaults = 0;
        iterator framesit;
        int index;
        for (auto i = sequence.begin(); i < sequence.end(); i++)
        {
            framesit = std::find(frames.begin(), frames.end(), *i);
            if (*framesit == *i)
            {
                /*page found*/
                index = framesit - frames.begin();
                arr[index] = true; // star
                pageFaults.push_back(false);
            }
            else if (frames.size() < noofFrames)
            {
                /*frames not full yet*/
                frames.push_back(*i);
                index = frames.size() - 1;
                arr[index] = true;
                it = (it + 1 - frames.begin() == noofFrames) ? frames.begin() : it + 1;
                pageFaults.push_back(false);
            }
            else
            {
                /*page fault*/

                pageFaults.push_back(true);
                noOfPageFaults++;
                index = it - frames.begin();
                while (arr[index]) // has a star
                {
                    arr[index] = false;
                    it = (it + 1 == frames.end()) ? frames.begin() : it + 1;
                    index = it - frames.begin();
                }
                /*no star*/
                *it = *i;
                arr[index] = true; // star
                it = (it + 1 == frames.end()) ? frames.begin() : it + 1;
            }

            /*record frames in trace*/
            trace.push_back(frames);
        }
        return trace;
    }


    /*getters for testing purposes*/
    std::string getAlgo()
    {
        return algo;
    }
    std::vector<int> getSequence()
    {
        return sequence;
    }
    int getNoofFrames()
    {
        return noofFrames;
    }
    std::vector<int> getFrames()
    {
        return frames;
    }
    Trace getTrace()
    {
        return trace;
    }
    std::vector<bool> getPageFaults()
    {
        return pageFaults;
    }
    int getNoOfPageFaults()
    {
        return noOfPageFaults;
    }

    /*setters for testing purposes*/
    void setNoofFrames(int a)
    {
        this->noofFrames = a;
    }
    void setAlgo(std::string a)
    {
        this->algo = a;
    }
    void setSequence(std::vector<int> a)
    {
        this->sequence = a;
    }
    /*for testing*/
    void setFrames(std::vector<int> a)
    {
        this->frames = a;
    }
};

class InputOutput
{
    public:
    PageReplacement inputAndLaunch()
    {
        int noFrames;
        std::string algo;
        std::vector<int> sequence;

        std::cin >> noFrames;
        std::cin >> algo;
        
        int in;
        std::cin >> in;
        while(in > -1)
        {
            sequence.push_back(in);
            std::cin >> in;
        }
        PageReplacement paging(noFrames,algo,sequence);
        paging.go();
        return paging;
    }

    void output()
    {}
};