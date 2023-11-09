// TOPIC: Condition Variable In C++ Threading

//IMPORTANT POINT: CV are used fpr two purposes
//     a. Notify other threads
//     b. Wait for some condition

// 
// NOTES:
// 1. Condition variables allow running threads to wait on some conditions and once those conditions are met
// the waiting thread is notified using :
//    a. notify_one();
//    b. notify_all();
// 2. You need mutex to use condition variable.
// 3. If some thread want to wait on some condition then it has to do these things:
//    a. Acquire the mutex lock using std::unique_lock<std::mutex> lock(m);.
//    b. Execute wait, wait_for or wait_until. The wait operations automatically release the mutex and suspend
//       the execution of the thread.
//    c. When the condition variable is notified, the thread is awakend, and the mutex is automatically reacquired.
//       The thread should then check the condition and resume waiting if the wake up was spurious.

// NOTE:
// 1. Condition variable is used to synchronize two or more threads.
// 2. Best use case of condition variable is Producer/Consumer problem.

/*
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int myAmount = 0;
std::mutex m;

void addMoney() {
	m.lock();
	++myAmount;
	m.unlock();
}

int main() {
	std::thread t1(addMoney);
	std::thread t2(addMoney);

	t1.join();
	t2.join();

	cout << myAmount << endl;
	return 0;
}


#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int myAmount = 0;
std::mutex m;

void addMoney() {
	std::lock_gaurd<std::mutex> lock(m);
	++myAmount;
}

int main() {
	std::thread t1(addMoney);
	std::thread t2(addMoney);

	t1.join();
	t2.join();

	cout << myAmount << endl;
	return 0;
}
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

std::condition_variable cv;
std::mutex m;
int balance = 0;

void addMoney(int money) {
	std::lock_guard<mutex> lg(m);
	balance += money;
	cout << "Amount Added Current Balance: " << balance << endl;
	cv.notify_one();
}

void withdrawMoney(int money) {
	std::unique_lock<mutex> ul(m);
	cv.wait(ul, [] { return (balance != 0) ? true : false; });
	if (balance >= money) {
		balance -= money;
		cout << "Amount Deducted: " << money << endl;
	}
	else {
		cout << "Amount Can't Be Deducted, Current Balance Is Less Than " << money << endl;
	}
	cout << "Current Balance Is: " << balance << endl;
}

int main() {
	std::thread t1(withdrawMoney, 500);
	std::thread t2(addMoney, 500);
	t1.join();
	t2.join();
	return 0;
}