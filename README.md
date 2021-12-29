# page-replacement
Page replacement algorithms for memory management in operating systems  

---  

## Pseudo code for predict algorithm

```C++ 

Predict(frames,sequence,it)
1    let q be a priority queue
    
2    for j = 0 to frames.size
3        i = find index of first occurence of p in sequence from it to sequence.size
4        q.insert(<i,j>)
    
5    return j from q.top (the index of the page in frames to be replaced)

```


