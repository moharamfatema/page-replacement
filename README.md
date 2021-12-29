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

## Pseudo code for clock  

```C++

clock()
    let arr = new vector<pair<accessed,frame itr>>
    where all accessed values = 0 //no star
    let it = arr.begin
    foundOrPlaced, it = find a page(page);
    if foundOrPlaced:
        find it in arr and set corresponding accessed = 1 //star
    else //page fault
        while( *it.accessed = 1 //star)
            *it.accessed = 0 //remove star
            it++ //move forward
        replace *it.itr
        set *it.accessed = 1 //star
        it++

```
