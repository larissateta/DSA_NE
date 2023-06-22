#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include<algorithm>
using namespace std;

// Help
void help() {
    cout << "--------------------------------------------------" << endl;
    cout << " * Command syntaxes  *" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "itemadd <item_id> <item_name> <quantity> <registration_date> " << endl;
    cout << "itemslist" << endl;
    cout << "help" << endl;
    cout << "exit" << endl;
}

// Changing to uppercase function
string toUpperCase(string str) {
    string result;
    for (char c : str) {
        result += toupper(c);
    }
    return result;
}

// Split line
vector<string> split(string str) {
    istringstream iss(str);
    vector<string> tokens;
    string token;

    while (getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

//Checks if id exists

bool idExist(string id) {
    string line;
    fstream file("data.csv", ios::in);

    while (getline(file, line)) {
        vector<string> splitLine = split(line);
        if (splitLine.size() > 0 && splitLine[0] == id) {
            return true;
        }
    }

    return false;
}

//validate date

bool isValidDate(string dateStr) {

    try {
        time_t now = time(0);
        struct tm today = *localtime(&now);
        string todayStr = to_string(today.tm_year + 1900) + "-" + to_string(today.tm_mon + 1) + "-" + to_string(today.tm_mday);

        return dateStr == todayStr;
    } catch (exception e) {
        return false;
    }
}

//validate quantity
bool isValidQuantity(string quantityStr) {
    try {
        int quantity = stoi(quantityStr);

        if (quantity < 0) {
            return false;
        }

        return true;
    } catch (exception e) {
        return false;
    }
}

//validate id
bool isValidId(string idStr) {

    try {
        int id = stoi(idStr);

        if (id < 0) {
            return false;
        }

        return true;
    } catch (exception e) {
        return false;
    }
}

// Add item
void addItem(string id, string item_name, string item_quantity, string item_registration_date) {
	try{
		string line;
    	fstream file("data.csv", ios::in);
	    fstream temp("temp.csv", ios::out | ios::app);
	    bool itemExist = false;
	    string newLine = id + "," + item_name + "," + item_quantity + "," + item_registration_date + "\n";
	    
	    if(idExist(id)){
	    	cout<<"ID already exists"<<endl;
		}else if(!isValidId(id)){
			cout<<"Invalid Id"<<endl;
			
		}else if(!isValidQuantity(item_quantity)){
			cout<<"Invalid Quantity"<<endl;
		}else{
			while (getline(file, line)) {
	        vector<string> splitLine = split(line);
	        if (splitLine.size() > 0 && splitLine[1] == item_name) {
	            cout << "Item already exists" << endl;
	            itemExist = true;
	            temp << newLine;
	            break;
	        } else {
	            temp << line << "\n";
	        }
	    }

	    if (!itemExist) {
	        temp << newLine;
	        cout << "Item added successfully!" << endl;
	    }

	    file.close();
	    temp.close();
	
	    remove("data.csv");
	    rename("temp.csv", "data.csv");
		}
	
	    
	}catch(exception e){
		cout<<"An error occured: "<<e.what()<<endl;
	}
    
}

//LIST ITEMS

void listItems() {
    fstream file("data.csv", ios::in);
    string line;
    vector<vector<string>> items;

    while (getline(file, line)) {
        vector<string> splitLine = split(line);
        if (splitLine.size() == 4) {
            items.push_back(splitLine);
        }
    }

    sort(items.begin(), items.end(), [](const vector<string>& item1, const vector<string>& item2) {
        return item1[1] < item2[1];
    });

    for (const vector<string>& item : items) {
        cout << "Item ID: " << item[0] << "\t\tItem Name: " << item[1] << "\t\tQuantity: " << item[2] << "\t\tReg Date: " << item[3] << endl;
    }

    file.close();
}


int main() {
    cout << "============================================================" << endl;
    cout << "Welcome to RCA Inventory System!" << endl;
    cout << "============================================================" << endl;
    cout << endl;

    cout << "Need help? Type 'Help' and press Enter key!" << endl;
    string input;
    while (true) {
        cout << "Console > ";
        getline(cin, input);

        string upperInput = toUpperCase(input);

        if (upperInput.find("ITEMADD") == 0) {
        	if(upperInput.length()== 7){
        		cout << "The command 'itemadd' is not complete. Please specify the item id, name, quantity, and registration date." << endl;
			}else{
				istringstream iss(upperInput);
            	string command, id, name, quantity, regDate;
            	iss >> command >> id >> name >> quantity >> regDate;

            	addItem(id, name, quantity, regDate);
			}
            
        } else if (upperInput == "ITEMSLIST") {
            listItems();
        } else if (upperInput == "HELP") {
            help();
        } else if (upperInput == "EXIT") {
            break;
        }else{
        	cout<<"Command not recognized, Type 'Help' to see the command Syntaxes"<<endl;
		}
    }

    return 0;
}

