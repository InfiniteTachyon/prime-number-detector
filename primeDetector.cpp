//DAVID TRAN

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

std::vector<int64_t>
detect_primes(const std::vector<int64_t> & nums, int n_threads);

// C++ barrier class
// -----------------------------------------------------------------------------
// Using personal barrier structure, as earlier gcc does not support cBarriers.
class simple_barrier {
  std::mutex m_;
  std::condition_variable cv_;
  int n_remaining_, count_;
  bool coin_;

  public:
  simple_barrier(int count = 1) { init(count); }
  void init(int count)
  {
    count_ = count;
    n_remaining_ = count_;
    coin_ = false;
  }
  bool wait()
  {
    if (count_ == 1) return true;
    std::unique_lock<std::mutex> lk(m_);
    if (n_remaining_ == 1) {
      coin_ = ! coin_;
      n_remaining_ = count_;
      cv_.notify_all();
      return true;
    }
    auto old_coin = coin_;
    n_remaining_--;
    cv_.wait(lk, [&]() { return old_coin != coin_; });
    return false;
  }
};



//Global Variables:
std::vector<int64_t> results;
std::vector<int64_t> input_global_vector;
int64_t inputPointer = -1;
std::atomic<bool> globalBreak = false;
std::atomic<bool> continueFlag = true;



void is_prime_t(int tid, int n_threads,simple_barrier &barrier) {
  
  int64_t n;
  while (1) {
    if (barrier.wait()) {   //Calls barrier to enter serial phase;

      if (continueFlag == false) {
        results.push_back(n);
        inputPointer++;
      }
      else {inputPointer++;}





      continueFlag=false;
      if ((long unsigned int)inputPointer<input_global_vector.size()) {
        n = input_global_vector.at(inputPointer);
        
        // Serial Algorithm
        if (n < 2) { continueFlag=true;}   
        else if (n <= 3) {results.push_back(n);  continueFlag=true;} // 2 and 3 are primes
        else if (n % 2 == 0) { continueFlag=true;} // handle multiples of 2
        else if (n % 3 == 0) { continueFlag=true;} // handle multiples of 3
      }
      else {globalBreak = true;}
      
    }
    barrier.wait();
    if (globalBreak) {break;}
    if (continueFlag) {continue;} 


    //Parallel Algorithm
    n = input_global_vector.at(inputPointer);
    int64_t max = sqrt(n);
    int64_t i = 5+6*tid;
    while (i<=max) {
      if (continueFlag) break;
      if (n%i==0) {continueFlag=true; break;}
      else if (n%(i+2)==0) {continueFlag=true; break;}
      i+=6*n_threads;
    }

  }





}




// detect_primes
// Precondition: List of numbers, int of threads
// Postcondition: returns a list of of only prime numbers
std::vector<int64_t>
detect_primes(const std::vector<int64_t> & nums, int n_threads)
{

  input_global_vector = nums;

  simple_barrier barrier(n_threads);    //Defining barrier wrapper
  std::vector<std::thread> threads;
  for (int i = 0; i<n_threads; i++) {
    threads.emplace_back(is_prime_t, i, n_threads,std::ref(barrier));
  }
  for( auto && t : threads)
    t.join();

  return results;
}



int main(int argc, char** argv) {
  std::vector<int64_t> nums;
  int threads = *argv[1] - '0';
  
  while (1) {
    int64_t num;
    if (!(std::cin >> num)) break;
    nums.push_back(num);
  }

  detect_primes(nums, threads);


  printf("primeDetector ran using %d threads.\n", threads);
  if (results.size() == 0) return 0;

  printf("Identified %d primes: \n", (int)results.size());
  for (int i = 0; i< (int)results.size(); i++) {
    std::cout<<" "<<results[i];
  }
  printf("\n");

  return 0;
}
