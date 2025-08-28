#pragma once
#include <mutex>
using namespace std;
////////////////////////////////////////////////////////////////////////
//
// Author: Jakob Tomas West
// Date: April 1st, 2023
//
// CS 6013
//
//
//
////////////////////////////////////////////////////////////////////////


#ifndef THREAD_SAFE_QUEUE_CONCURRENTQUEUE_H
#define THREAD_SAFE_QUEUE_CONCURRENTQUEUE_H

template <typename T>
class ConcurrentQueue {

public:
    ConcurrentQueue() :
            head_( new Node{ T{}, nullptr } ), size_( 0 )
    {
        tail_ = head_;
    }

    void enqueue( const T & x ) {
        std::unique_lock<std::mutex> lock(mutex_);
        //create a new node (holds address allocated Node) with value passed in x and assign next pointer as null
        Node* newNode = new Node{x, nullptr};
        // if the size of the queue is 0 make dummy head point to newNode
        if (size_ ==0){
            head_->next = newNode;
        } else {
            //from null to ----> newNode
            tail_->next = newNode;
        }
        //rename newNode to be the tail
        tail_ = newNode;
        size_++;
    }
    //note if head pointer and tail pointer are = we are empty and pointing to dummy node
    //tail pointer is not used during dequeue event w/ dummy implementation
    bool dequeue( T* ret ) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (size_ == 0) {
            return false;
        }
        //dummys next is node to remove
        Node* temp = head_->next; // could be head_ -> next -> data
        *ret = temp->data;
        //make head point to where it's next node was pointing to we are still linked
        head_->next = temp->next;
        if (size_ == 1) {
            tail_ = head_; //reset tail to the dummy node when queue is empty -- new dnummy
        }
        //get rid of og head next node (dequeue)
        delete temp;
        --size_;
        return true;

    }

    ~ConcurrentQueue() {
        std::unique_lock<std::mutex> lock(mutex_);
        while( head_ != nullptr ) {
            Node* temp = head_->next;
            delete head_;
            head_ = temp;
        }
    }

    int size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return size_;
    }

private:

    struct Node {
        T      data;
        //next pointer
        Node * next;
    };

    Node * head_;
    Node * tail_;
    int    size_;
    mutable mutex mutex_;
};
#endif //THREAD_SAFE_QUEUE_CONCURRENTQUEUE_H
