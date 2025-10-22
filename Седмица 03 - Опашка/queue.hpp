#ifndef QUEUE_HPP
#define QUEUE_HPP

template <typename T>
class Queue {
public:
  virtual void enqueue(const T&) = 0;
  virtual void dequeue() = 0;
  virtual T& first() = 0; 
  virtual const T& first() const = 0;
  virtual bool empty() const = 0;
  
  virtual ~Queue() = default;
};

#endif