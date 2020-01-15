#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//struct to keep one driver's info
struct Driver {
	string name;
	int lap_time;
	Driver* next;
};

//This function adds drivers to linkedlist in a sorted manner
Driver* addSorted(Driver* head, string name, int lap) {
	//If list is empty, simply add to the beginning(head) and return head
	if (head == nullptr) {
		head = new Driver();
		head->lap_time = lap;
		head->name = name;
		head->next = nullptr;
		return head;
	}
	if (head->lap_time > lap) {//if list is not empty but the one to be added has better lap time than head, then add it to beginning again and return head.
		Driver* toAdd = new Driver();
		toAdd->name = name;
		toAdd->lap_time = lap;
		toAdd->next = head;
		head = toAdd;
		return head;
	}
	//We will iterate through the linkedlist. We need to keep track of two pointers.
	Driver* prev = head;//previous
	Driver* temp = head;//current
	while (temp != nullptr && temp->lap_time<=lap) {//If current lap time is smaller than the one to be added, proceed to next item.
		prev = temp;
		temp = temp->next;
	}
	//At this point we have the place to add. It is either between prev and temp or just simply the tail of list.
	Driver* toAdd = new Driver();//Create a new node and enter its info.
	toAdd->name = name;
	toAdd->lap_time = lap;
	if (temp == nullptr) {//If we must add it to the end
		temp = toAdd;
		prev->next = temp;
		temp->next = nullptr;
		return head;
	}
	//otherwise we add between prev and temp
	prev->next = toAdd;
	toAdd->next = temp;
	return head;
}

Driver* deleteDriver(Driver* head, string name) {//This function deletes a driver from the linkedlist
	if (head->name == name) {//If we need to delete head, make next one the new head before. And then return it.
		Driver *temp = head;
		head = head->next;
		delete temp;
		return head;
	}
	Driver* prev = head;//previous
	Driver* temp = head;//current
	while (temp != nullptr && temp->name != name) {//If current is not the target, proceed
		prev = temp;
		temp = temp->next;
	}
	//At this point we have the place of the one we need to delete. It is temp.
	if (temp->next == nullptr) {//If temp is at the last, make prev last and delete temp.
		prev->next = nullptr;
		delete temp;
		return head;
	}//Otherwise it is in between head and tail. Tie prev and temp's next to free temp and then delete temp.
	prev->next = temp->next;
	delete temp;
	return head;
}

//This function determines the action we should take once a line is read from the file.
int whatToDo(Driver* head, string name, int lap) {
	if (head == nullptr)//If list is empty
		return 0;//add
	Driver* temp = head;
	while (temp != nullptr) {//Iterate through list
		if (temp->name == name && temp->lap_time > lap)//If we found driver and it has worse lap time in the list.
			return 1;//update
		else if (temp->name == name && temp->lap_time <= lap)//If we found driver and it has better lap time in the list.
			return 2;//do nothing
		temp = temp->next;
	}
	return 0;//if nothing is found, then add
}

//This function searches driver and find his/her position in the race as well as personal best of lap times.
int position(Driver* head, string name, int & personal_best) {
	Driver* temp = head;
	int pos = 1;//First position
	while (temp != nullptr) {//Iterate through the list
		if (temp->name == name) {//If driver is found,
			personal_best = temp->lap_time;//return personal best
			return pos;//and return pos
		}
		temp = temp->next;
		pos++;
	}
}

int main() {
	//File name acquiring
	string file;
	ifstream i;
	cout << "Please enter a file name." << endl;
	cin >> file;
	i.open(file.c_str());
	while (i.fail()) {
		cout << "Unable to open file " << file << endl << "Please enter a different file name." << endl;
		cin >> file;
		i.open(file.c_str());
	}
	cout << "Successfully opened file " << file << endl;
	//Qualifying Laps
	cout <<endl<<"###############################" << endl << "Qualifying Laps :" << endl <<"###############################" << endl;
	Driver* head = nullptr;
	string line;
	while (getline(i, line)) {//For each line
		istringstream iss(line);//Make it a stream
		string name;
		int lap;
		iss >> name >> lap;//extract info from the line
		cout << name <<" completed the lap in "<< lap <<" milliseconds" << endl;//Inform user about this lap
		int action = whatToDo(head, name, lap);//Determine the action
		if (action == 0) {
			head = addSorted(head, name, lap);//Simple addition
		}
		else if (action == 1) {//Updating
			head = deleteDriver(head, name);//First delete
			head = addSorted(head, name, lap);//Then add again
		}
		int personal_best = 0;
		int pos = position(head, name, personal_best);//Find pos and PB
		cout << name<<": current personal best is "<<personal_best<<"; current position is " <<pos << endl;//Inform the user about driver
	}
	//Print the results!!
	cout <<endl << "###############################" << endl << "Results:" << endl << "###############################" << endl;
	Driver* temp = head;
	int count = 1;
	while (temp != nullptr) {
		cout << count << ". " << temp->name << " " << temp->lap_time << endl;
		temp = temp->next;
		count++;
	}
	return 0;
}