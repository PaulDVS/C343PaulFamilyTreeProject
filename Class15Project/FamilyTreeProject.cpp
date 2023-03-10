#include<iostream>
#include<string>

using namespace std;

int currYear = 2023;
int currMonth = 3;
int currDay = 10;

struct familyMemberNode{
	int id;
	string name;
	int birthYear;
	int birthMonth;
	int birthDay;
	struct familyMemberNode *Spouse;
	struct familyMemberNode *Sibling;
	struct familyMemberNode *Child;
	
	void displayFullFamily(int level){
		for(int i=0; i<level; i++){
			cout<<"    ";
		}
		cout<<"ID: "<<id<<", ";
		cout<<name<<", ";
		cout<<"Age: "<<this->getAge()<<", ";
		if(Spouse != NULL){
			cout<<"Married to "<<Spouse->name<<", ";
			cout<<"Age: "<<Spouse->getAge()<<".";
		}
		cout<<endl;
		if(Child !=NULL){
			Child->displayFullFamily(level+1);
		}
		if(Sibling !=NULL){
			Sibling->displayFullFamily(level);
		}
	}
	
	int getAge(){
		int age;
		
		age = currYear-birthYear;
		if(birthMonth > currMonth){
			age--;
		}
		else if(birthMonth == currMonth){
			if(birthDay > currDay){
				age--;
			}
		}
		return age;
	}
	
	familyMemberNode* checkId(int checkId){
		familyMemberNode *target = NULL;
		if(id == checkId){
			return this;
		}
		target = Child->checkId(checkId);
		if(target != NULL){
			return target;
		}
		target = Sibling->checkId(checkId);
		return target; //Either target contains a value, or no-one here has the id and returns null.
	}
};

class familyTree{
	private:
		struct familyMemberNode *Ancestor;
		int nextId;
	
	public:
		familyTree(familyMemberNode *inputMember){
			Ancestor = inputMember;
			nextId = 1;
		}
		
		int getNextId(){
			return nextId;
		}
		
		familyMemberNode* getMemberById(int Id){
			return Ancestor->checkId(Id);
		}
		
		int getAge(int Id){
			familyMemberNode *currNode = this->getMemberById(Id);
			int age = currNode->getAge();
		}
		
		void addSpouse(int Id, familyMemberNode *inputMember){
			familyMemberNode *currNode = this->getMemberById(Id);
			
			//Checks spouse isn't younger than children
			if(currNode->Child != NULL && currNode->Child->getAge() > inputMember->getAge()){
				cout<<"You cannot marry someone younger than your children."<<endl;
				cout<<"Cancelling..."<<endl;
				return;
			}
			else if(currNode->Child != NULL && currNode->Child->getAge() == inputMember->getAge()){
				cout<<"You cannot marry someone the same age as your children."<<endl;
				cout<<"Cancelling..."<<endl;
				return;
			}
			
			
			if(currNode->Spouse != NULL){
				cout<<currNode->name<<" has re-married to "<< inputMember->name <<endl;
				currNode->Spouse = inputMember;
			}
			else{
				cout<<currNode->name<<" has married "<< inputMember->name <<endl;
				currNode->Spouse = inputMember;
			}
		}
		
		void addChild(int Id, familyMemberNode *inputMember){
			
			
			familyMemberNode *currNode = this->getMemberById(Id);
			familyMemberNode *temp;
			int oldestChange = 0;
			
			//Check children aren't older than parents
			if( (currNode->getAge() < inputMember->getAge()) || (currNode->Spouse != NULL && currNode->Spouse->getAge() < inputMember->getAge()) ){
				cout<<"Children cannot be older than their parents."<<endl;
				cout<<"Cancelling..."<<endl;
				return;
			}
			else if( (currNode->getAge() == inputMember->getAge()) || (currNode->Spouse != NULL && currNode->Spouse->getAge() == inputMember->getAge()) ){
				cout<<"Children cannot be the same age as their parents."<<endl;
				cout<<"Cancelling..."<<endl;
				return;
			}
			
			//compare children.
			inputMember->Sibling = currNode->Child;

			while(inputMember->Sibling !=NULL){

				if(inputMember->birthYear > inputMember->Sibling->birthYear){
					temp = inputMember->Sibling->Sibling;
					inputMember->Sibling->Sibling = inputMember;
					inputMember->Sibling = temp;
					oldestChange = 1;
				}
				else if(inputMember->birthYear == inputMember->Sibling->birthYear){
					//If same year check oldest by month. 
					if(inputMember->birthMonth > inputMember->Sibling->birthMonth){
						temp = inputMember->Sibling->Sibling;
						inputMember->Sibling->Sibling = inputMember;
						inputMember->Sibling = temp;
					}
					else if(inputMember->birthMonth == inputMember->Sibling->birthMonth){
						//If same month check oldest by day.
						if(inputMember->birthDay > inputMember->Sibling->birthDay){
							temp = inputMember->Sibling->Sibling;
							inputMember->Sibling->Sibling = inputMember;
							inputMember->Sibling = temp;
							oldestChange = 1;
						}
						else{
							break; //If same day, just leave as is.
						}
					}
					else{
						break; //If no Change, exit while loop
					}
				}
				else{
					break; //If no Change, exit while loop
				}
			}
			
			if(oldestChange == 0){ //If inputMember is the oldest, make them the first child of currNode
				currNode->Child = inputMember;
			}
			nextId++;
			
		}
		
		void updateMember(int id){
			string name;
			int birthYear, birthMonth, birthDay;
			
			familyMemberNode *currNode = this->getMemberById(id);
			int checkLoop = 0;
			
			cout<<"Please enter "<< currNode->name << "'s new details: "<<endl;
			cin.ignore();
			
			cout<<"Please enter Name: "<<endl;
			getline(cin, name);
			currNode->name = name;
			
			do{
				checkLoop = 0;
				
				cout<<"Please enter birth year: "<<endl;
				cin>>birthYear;
				currNode->birthYear = birthYear;
				
				cout<<"Please enter birth month (in numerical format): "<<endl;
				cin>>birthMonth;
				currNode->birthMonth = birthMonth;
				
				cout<<"Please enter birth day: "<<endl;
				cin>>birthDay;
				currNode->birthDay = birthDay;
				
				
				if(currNode->getAge()<0){
					cout<<"Error, date hasn't happened yet."<<endl;
					cout<<endl;
					checkLoop =1;
				}
				if(currNode->Spouse != NULL &&  currNode->getAge()<16 ){
					cout<<"Current family member is married, You cannot set their age to be below 16.";
					cout<<endl;
					checkLoop =1;
				}
				if(currNode->Child != NULL &&  currNode->getAge() < currNode->Child->getAge() ){
					cout<<"Current family member has a child, cannot set their age to be below their child's age.";
					cout<<endl;
					checkLoop =1;
				}
				else if(currNode->Child != NULL &&  currNode->getAge() == currNode->Child->getAge() ){
					cout<<"Current family member has a child, cannot set their age to be equal to their child's age.";
					cout<<endl;
					checkLoop =1;
				}
				
			}while(checkLoop!=0);
			
		}
		
		int displayNuclearFamily(int Id){
			familyMemberNode *currNode = this->getMemberById(Id);
			
			cout<<"ID: "<<currNode->id<<", ";
			cout<<currNode->name<<", ";
			cout<<"Age: "<<currNode->getAge()<<", ";
			if(currNode->Spouse != NULL){
				cout<<"Married to "<<currNode->Spouse->name<<", ";
				cout<<"Age: "<<currNode->Spouse->getAge()<<".";
			}
			cout<<endl;
			currNode = currNode->Child;
			while(currNode!=NULL){
				cout<<"    ";
				
				
				cout<<"ID: "<<currNode->id<<", ";
				cout<<currNode->name<<", ";
				cout<<"Age: "<<currNode->getAge()<<", ";
				if(currNode->Spouse != NULL){
					cout<<"Married to "<<currNode->Spouse->name<<", ";
					cout<<"Age: "<<currNode->Spouse->getAge()<<".";
				}
				cout<<endl;
				
				currNode = currNode->Sibling;
			}
		}
		
		int displayFullFamily(){
			familyMemberNode *currNode = Ancestor;
			
			cout<<endl;
			cout<<"Full Family Tree"<<endl;
			currNode->displayFullFamily(0);
		}
};

familyMemberNode* createFamilyMember(int Id){
	string name;
	int birthYear, birthMonth, birthDay;
	
	familyMemberNode *familyMember = new familyMemberNode();
	familyMember->Spouse = NULL;
	familyMember->Sibling = NULL;
	familyMember->Child = NULL;
	familyMember->id = Id;
	
	cout<<"Please enter Name: "<<endl;
	getline(cin, name);
	familyMember->name = name;
	
	do{
		cout<<"Please enter birth year: "<<endl;
		cin>>birthYear;
		familyMember->birthYear = birthYear;
		
		cout<<"Please enter birth month (in numerical format): "<<endl;
		cin>>birthMonth;
		familyMember->birthMonth = birthMonth;
		
		cout<<"Please enter birth day: "<<endl;
		cin>>birthDay;
		familyMember->birthDay = birthDay;
		
		
		if(familyMember->getAge()<0){
			cout<<"Error, date hasn't happened yet."<<endl;
			cout<<endl;

		}
	}while(familyMember->getAge()<0);
	
	return familyMember;
}



int main()
{
	int ch;
	int id;
	familyMemberNode *temp;
	
	//Init family tree
	cout<<"Please enter ancestor's details: "<<endl;
	familyMemberNode *ancestor = createFamilyMember(0);
	familyTree myFamily(ancestor);
	
	//Start menu
	
	do
	{
		cout<<"\n 1 Add Spouse "<<endl;
		cout<<"\n 2 Add Child "<<endl;
		cout<<"\n 3 Update (Update spouse with Add Spouse) "<<endl;
		cout<<"\n 4 Display Nuclear Family "<<endl;
		cout<<"\n 5 Display Full Family "<<endl;
		cout<<"\n 9 Exit "<<endl;
		cin>>ch;
		switch(ch)
		{
			case 1:
				cout<<"Please enter the id of the targer family member:";
				cin>>id;
				if(id>=myFamily.getNextId()){
					cout<<"Error, Family member doesn't exist"<<endl;
					cout<<"Cancelling..."<<endl;
					break;
				}
				if(myFamily.getAge(id) < 16){
					cout<<"A person has to be 16 years or older to get married."<<endl;
					cout<<"Cancelling..."<<endl;
					break;
				}
				
				cout<<"Please enter spouse's details: "<<endl;
				cin.ignore();
				temp = createFamilyMember(0);
				
				if(temp->getAge() < 16){
					cout<<"A person has to be 16 years or older to get married."<<endl;
					cout<<"Cancelling..."<<endl;
					break;
				}
				
				myFamily.addSpouse(id, temp);
				break;
			
			case 2:
				cout<<"Please enter the id of the targer family member:";
				cin>>id;
				if(id>=myFamily.getNextId()){
					cout<<"Error, Family member doesn't exist"<<endl;
					cout<<"Cancelling..."<<endl;
					break;
				}
				
				cout<<"Please enter child's details: "<<endl;
				cin.ignore();
				temp = createFamilyMember(myFamily.getNextId());
				
				myFamily.addChild(id, temp);
				break;
				
			case 3:
				cout<<"Please enter the id of the targer family member:";
				cin>>id;
				
				if(id>=myFamily.getNextId()){
					cout<<"Error, Family member doesn't exist"<<endl;
					cout<<"Cancelling..."<<endl;
					break;
				}
				
				myFamily.updateMember(id);
				break;
				
			case 4:
				cout<<"Please enter the id of the targer family member:";
				cin>>id;
				if(id>=myFamily.getNextId()){
					cout<<"Error, Family member doesn't exist"<<endl;
					cout<<"Cancelling..."<<endl;
					break;
				}
				
				myFamily.displayNuclearFamily(id);
				break;
				
			case 5:
				myFamily.displayFullFamily();
				break;
				
			case 9:
				cout<<"Thank you for using this program"<<endl;
				break;
				
			default:
				cout<<"Error incorrect input";
				break;	
		}
	}while(ch!=9);
	
}
