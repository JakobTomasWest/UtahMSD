////
//// Created by Jakob West on 4/1/24.
////
//#include "SerialQueue.hpp"
//#include <iostream>
//
//void testQueue(SerialQueue<int>& queue, int iterations) {
//    for (int i = 0; i < iterations; ++i) {
//        queue.enqueue(i);
//        if(queue.size()==1)
//            cout << "size = " << queue.size() <<" after enqueuing " << i+1 << " iterations \n";
//
//    }
//    int out;
//    for (int i = 0; i < iterations; ++i) {
//        bool result = queue.dequeue(&out);
//        if (!result || out != i) {
//            cout << "Error in dequeue. Expected: " << i << ", got: " << out << "\n";
//            return;
//        }
//        if(queue.size()==1){
//            cout << "size = " << queue.size() <<" after dequeuing " << i+1 << " iterations \n";
//        }
//    }
//    cout << "Test passed. " << iterations << " elements enqueued and dequeued successfully." << "\n";
//}
//
//int main() {
//    SerialQueue<int> queue;
//    testQueue(queue, 100); // Test with 100 iterations
//    return 0;
//}
