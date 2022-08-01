# prime-number-detector


## Inspiration
At the time of making this, I was inspired to make code faster (via multithreading) as well as encryption. So, I thought the creation of said project would be a good aid in understanding both concepts!

## What did I learn?
Throughout this project, I learned how to effectively use mutexes, barriers and atomic variables to maximize parrellel runtime; maximizing speeds.

## Challenges
Greatest challenge in this project was reducing the amount of barriers used. For example, I originally used 4 barriers, but I found that to be taxing on the speed on higher inputs. I eventually realized all the parallel processes could be executed in one instance of a barrier, and all the serial processes could be executed in another, negating the need to have more than 2 barriers.

## How do I run this?
To run this, simply clone this repository using:
```
 $ git clone https://github.com/InfiniteTachyon/prime-number-detector.git
```
and compile the code using:
```
 $ g++ -Wall -O2 primeDetector.cpp -o primeDetect -pthread -lm
```

You can then run the code using an appropriate thread number and input file as shown here:
```
 $ ./primeDetect 8 < input1.txt
 Identified 7 primes: 
  2 3 5 7 101 103 107
```

## Future Updates?
For the most part, this project is complete, but there are some updates that I'd like to make when I have time:
 - Error Handling
   - Invalid Inputs Handling
