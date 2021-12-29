#include <vector>
#include <string>
#include <algorithm>
#include <bits/stdc++.h>

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
        if (std::find(frames.begin(), frames.end(), *it) != frames.end())
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
        return trace;
    }
    Trace clock()
    {
        std::vector<std::pair<bool,iterator>> arr;
        arr.resize(noofFrames);
        for(auto j = arr.begin(); j < arr.end(); j++)
        {
            j->first = 0; //no stars
        }
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

    /*this stays public*/
    void output();

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