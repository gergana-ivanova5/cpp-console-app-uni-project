// SardechenPuls.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include<list>
#include<fstream>
#include<algorithm>
using namespace std;

class HeartRate {
private:
	int poredenMoment;
	int HR;

public:
	HeartRate() {
		poredenMoment;
		HR;
	}

	HeartRate(int p, int h) {
		poredenMoment = p;
		HR = h;
	}

	HeartRate(const HeartRate &obj) {
		poredenMoment = obj.poredenMoment;
		HR = obj.HR;
	}

	int getPoredenMoment() {
		return poredenMoment;
	}

	int getHR() {
		return HR;
	}
	
	void setPoredenMoment(int pm) {
		poredenMoment = pm;
	}

	void setHR(int hr) {
		HR = hr;
	}

	friend bool operator <(HeartRate& hr1, HeartRate& hr2);
	friend bool operator >(HeartRate& hr1, HeartRate& hr2);
	friend istream& operator >> (istream& istr, HeartRate& testHeartRate);// потоков вход
	friend ostream& operator << (ostream& ostr, const HeartRate& testHeartRate); //потоков изход

	//статична функция за сравнение между два подадени обекта от класа HeartRate за по-малко по пораден момент
	static bool lessThan(HeartRate hr1, HeartRate hr2) // вика се с HeartRate::lessthan()
	{
		if (hr1.poredenMoment < hr2.poredenMoment)
		{
			return true;
		}
		else {
			return false;
		}
	}

};

bool operator <(HeartRate& hr1, HeartRate& hr2)
{
	return (hr1.HR < hr2.HR);
}

bool operator >(HeartRate& hr1, HeartRate& hr2)
{
	return (hr1.HR > hr2.HR);
}

istream& operator>>(istream& istr, HeartRate& testHeartRate)
{
	//cout << "Please enter Poreden Moment and Heart Rate here in the same order: " << endl;
	istr >> testHeartRate.poredenMoment >> testHeartRate.HR;
	return istr;
}

ostream& operator << (ostream& ostr, const HeartRate& testHeartRate)
{
	ostr << testHeartRate.poredenMoment <<" "<<testHeartRate.HR << endl;
	return ostr;
}

class lessThanNum { //functor
private:
	int num;
public:
	lessThanNum(int n) :num(n){}

	bool operator() (HeartRate hr) const
	{
		return hr.getHR() <= num;
	}

};

class moreThanNum { //functor
private:
	int num;
public:
	moreThanNum(int n) :num(n) {}

	bool operator() (HeartRate hr) const
	{
		return hr.getHR() >= num;
	}

};

class betweenTwoNum { //functor
private:
	int O1, O2;
public:
	betweenTwoNum(int num1, int num2) :O1(num1), O2(num2) {}

	bool operator() (HeartRate hr) const
	{
		int p = hr.getHR();
		return (p <= O1 || p>=O2);
	}

};

class HeartData {
private:
	string patientName;
	list<HeartRate> heartRates;
	static list<HeartRate>::iterator myIt;

public:
	HeartData(string filename)
	{
		fstream file;
		string namePatient;
		int allHeartRates;
		HeartRate hr;
		file.open(filename, ios::in);
		try {
			if (file)
			{
				//cout << "Pleaese enter patient name here: ";
				file >> namePatient;
				patientName = namePatient;
				//cout << "Pls enter the number of entries for " << patientName << " here: ";
				/*file >> allHeartRates;
				heartRates.resize(allHeartRates);*/

				while(!file.eof()){
					file >> hr;
					this->heartRates.push_back(hr);
				}
			}
			else {
				throw("File NOT found!");
			}
		}
		catch (exception& ex) {
			cout << "Exception occurred!" << endl;
			}
		file.close();
	}

	string getPatientName()
	{
		return patientName;
	}

	void setPatientName(string pn)
	{
		patientName = pn;
	}

	list<HeartRate> getHeartRates() 
	{
		return heartRates;
	}
	static list<HeartRate> getLowerHeartRatesList(HeartData HD, int number)
	{
		//статитчна функция - по подаден обект от класа HeartData и число се връща списък с обекти от класа HeartRate, 
		//в който HR са по-малки от подаденото число

		list<HeartRate> modifiedList = HD.getHeartRates();
		
		modifiedList.remove_if(moreThanNum(number));//трие елементите от колекцията, за които предикатът
		//връща true
	
		return modifiedList;
	}

	static list<HeartRate> getHigherHeartRatesList(HeartData HD, int number)
	{
		//статитчна функция - по подаден обект от класа HeartData и число се връща списък с обекти от класа HeartRate, 
		//в който HR са по-големи от подаденото число

		list<HeartRate> modifiedList = HD.getHeartRates();

		modifiedList.remove_if(lessThanNum(number));//трие елементите от колекцията, за които предикатът
		//връща true

		return modifiedList;
	}

	static list<HeartRate> getHeartRatesList(HeartData HD, int O1, int O2)
	{
		//статитчна функция - по подаден обект от класа HeartData и две числа O1 и O2 се изчислява/връща списък с обекти от класа HeartRate, 
		//в който HR са по-големи от O1 и по-малки от O2 

		list<HeartRate> modifiedList = HD.getHeartRates();

		modifiedList.remove_if(betweenTwoNum(O1,O2));//трие елементите от колекцията, за които предикатът
		//връща true
		return modifiedList;
	}
	
	void sortHeartRates()
	{
		heartRates.sort(HeartRate::lessThan);
	}
	friend ostream &operator<<(ostream &ostr, const HeartData& hd);
	friend istream &operator>>(istream& istr, HeartData& hd);
};

ostream& operator<<(ostream& ostr, const HeartData& hd)
{
	ostr << "Patient name: " << hd.patientName << endl;
	copy(hd.heartRates.begin(), hd.heartRates.end(), ostream_iterator<HeartRate>(ostr, " "));
	return ostr;
}

istream& operator>>(istream& istr, HeartData& hd)
{
	cout << "Please enter patient name here: ";
	istr >> hd.patientName;
	int heartRatesNumber;
	cout << "how many Heart Rates will you enter? Answer: ";
	istr >> heartRatesNumber;
	if (heartRatesNumber <= 0)
	{
		throw("Invalid number!");
	}
	else {
		hd.heartRates.resize(heartRatesNumber);
		list<HeartRate>::iterator it = hd.heartRates.begin();
		while (it != hd.heartRates.end())
		{
			istr >> *it;
			it++;
		}

	}
	return istr;
}

int main()
{
	HeartData object1("test.txt");
	//извежда информацията за обекта с потоков изход, със соритиране по пореден момент
	cout << "Izvejdane bez sortirane: " << endl;
	cout << object1;
	cout << endl;
	cout << "Izvejdane sas sortirane: " << endl;
	object1.sortHeartRates();
	cout << object1;
	cout << endl;
	cout << "Rezultat ot tretata statichna funkciq: " << endl;
	list<HeartRate> myList = HeartData::getHeartRatesList(object1, 80,100);
	copy(myList.begin(), myList.end(), ostream_iterator<HeartRate>(cout, " "));
	cout << endl;

	cout << "Rezultat ot vtorata statichna funkciq: " << endl;
	list<HeartRate> myList1 = HeartData::getHigherHeartRatesList(object1, 80);
	copy(myList1.begin(), myList1.end(), ostream_iterator<HeartRate>(cout, " "));
	cout << endl;

	try {
		cout << "Rezultat ot parvata statichna funkciq: " << endl;
		list<HeartRate> myList2 = HeartData::getLowerHeartRatesList(object1, 70);
		copy(myList2.begin(), myList2.end(), ostream_iterator<HeartRate>(cout, " "));
	}
	catch (exception &x)
	{
		cout << "Exception found!";
	}
	system("pause");
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
