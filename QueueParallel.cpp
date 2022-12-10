// QueueParallel.cpp: define el punto de entrada de la aplicación.
//Massiel Oviedo Sivincha EDA-lab 

// Copyright

#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <condition_variable>

template <typename T>
class ConcurrentQueue {
public:
    void push(const T& data) { 
        //queue_.push(data); //this 
        //usar condition_variable es una forma de sincronizar los mutex
        std::unique_lock<std::mutex> mlock(queue_mutex_);
        queue_.push(data);
        mlock.unlock();
        cond_.notify_one();//notifica a las otras threads cunado existan otro elemento
        

    }

    T pop() {
        //T result = queue_.front();//this
      
        std::unique_lock<std::mutex> mlock(queue_mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);//esperar por un mutex
        }
        T result = queue_.front();
        queue_.pop();
        return result;
        
    }

private:
    std::queue<T> queue_;
    std::mutex queue_mutex_;
    std::condition_variable cond_;
};

class Producer {
public:
    Producer(unsigned int id, ConcurrentQueue<std::string>* queue)
        : id_(id), queue_(queue) {}

    void operator()() {
        int data = 0;
        while (true) {
            std::stringstream stream;
            stream << "Producer: " << id_ << " Data: " << data++ << std::endl;
            queue_->push(stream.str());
            std::cout << stream.str() << std::endl;
        }
    }

private:
    unsigned int id_;
    ConcurrentQueue<std::string>* queue_;
};

class Consumer {
public:
    Consumer(unsigned int id, ConcurrentQueue<std::string>* queue)
        : id_(id), queue_(queue) {}

    void operator()() {
        while (true) {
            std::stringstream stream;
            stream << "Consumer: " << id_ << " Data: " << queue_->pop().c_str()
                << std::endl;

            std::cout << stream.str() << std::endl;
        }
    }

private:
    unsigned int id_;
    ConcurrentQueue<std::string>* queue_;
};

int main(int argc, char* argv[]) //argv cantidad de variables  y argv las variables
{
    if (argc != 3) {//si no recibimos 3 argumentsso no hacemos nada
        return 1;
    }
    int number_producers = std::stoi(argv[1]);
    int number_consumers = std::stoi(argv[2]);

    std::cout << "input:" << number_producers << " " << number_consumers <<std::endl;


    ConcurrentQueue<std::string> queue;

    std::vector<std::thread*> producers;
    for (unsigned int i = 0; i < number_producers; ++i) {
        std::thread* producer_thread = new std::thread(Producer(i, &queue));
        producers.push_back(producer_thread);
    }

    std::vector<std::thread*> consumers;
    for (unsigned int i = 0; i < number_consumers; ++i) {
        std::thread* consumer_thread = new std::thread(Consumer(i, &queue));
        consumers.push_back(consumer_thread);
    }

    int stop;
    std::cin >> stop;

    for (int i = 0; i < number_consumers; i++) {
        //producers[i].join();
        for (int j = 0; j < number_consumers; i++) {
            consumers[j][i].join();
        }
    }
    for (int i = 0; i < number_producers; i++) {
        //producers[i].join();
        for (int j = 0; j < number_producers; i++) {
            producers[j][i].join();
        }
    }
    // join
    //producers.join();

    return 0;
}
