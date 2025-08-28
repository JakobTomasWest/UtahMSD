#include <iostream>
#include <sys/time.h>
#include <cstring>
#include <stdio.h>
extern "C" {

  void _sayHello();
  int _myPuts(const char* s, int len);
  struct timeval _myGTOD();
}

int main(){
	using namespace std;
	_sayHello();
        struct timeval theTime = _myGTOD();
	cout << "My Seconds: " << theTime.tv_sec << endl;
        cout << "My Microseconds: " << theTime.tv_usec << endl;
	struct timeval tv_gettimeofday;
	struct timezone tz;
	gettimeofday(&tv_gettimeofday, &tz);
	cout << "Seconds: " << tv_gettimeofday.tv_sec << ", Microseconds: " << tv_gettimeofday.tv_usec << endl;
	_myPuts("myPuts works", 12);
	return 0;
}
