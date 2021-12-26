#include <vector>
#include <string>
#include <deque>
#include <algorithm>

class PageReplacement
{
    unsigned int noofFrames;
    std::string algo;
    std::vector<unsigned int> sequence;
    std::deque<unsigned int> frames;
    std::vector<std::deque<unsigned int>> trace;
    std::vector<bool> pageFaults;

public:
    PageReplacement(
        const unsigned int noofFrames,
        const std::string algo,
        const std::vector<unsigned int> sequence)
    {
        init(noofFrames,algo,sequence);
    }

    void init(
        const unsigned int noofFrames,
        const std::string algo,
        const std::vector<unsigned int> sequence)
    {
        this->noofFrames = noofFrames;
        this->algo = algo;
        this-> sequence=sequence ;
    }
    
    void optimal();
    std::vector<std::deque<unsigned int>> fifo()
    {
        auto it = frames.begin();
        for (int i = 0; i < sequence.size(); i++)
        {
            /*search for page in frames*/
            if (std::find(frames.begin(), frames.end(), sequence[i]) != frames.end())
            {
                /*page found*/
                pageFaults.push_back(false);
            }
            else if (frames.size() < noofFrames)
            {
                /*frames are not full yet*/
                frames.push_back(sequence[i]);
                pageFaults.push_back(false);
            }
            else
            {
                /*page fault*/
                pageFaults.push_back(true);
                *it = sequence[i];
                it++;
                if(it == frames.end()) it = frames.begin();

            }
            /*record frames in trace*/
            trace.push_back(frames);
        }
        return trace;
    }
    void lru();
    void clock();
    void output();
    std::string getAlgo() { return algo; }
    std::vector<unsigned int> getSequence() { return sequence; }
    const unsigned int getNoofFrames() { return noofFrames; }
    std::deque<unsigned int> getFrames() { return frames; }
    std::vector<std::deque<unsigned int>> getTrace() { return trace; }
    std::vector<bool> getPageFaults(){return pageFaults;}
};