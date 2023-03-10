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
		
		void addSpouse(int Id, familyMemberNode *inputMember){
			familyMemberNode *currNode = this->getMemberById(Id);
			
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
			nextId++;
			
			
			familyMemberNode *currNode = this->getMemberById(Id);
			familyMemberNode *temp;
			int oldestChange = 0;
			
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
	
	cout<<"Please enter birth year: "<<endl;
	cin>>birthYear;
	familyMember->birthYear = birthYear;
	
	cout<<"Please enter birth month (in numerical format): "<<endl;
	cin>>birthMonth;
	familyMember->birthMonth = birthMonth;
	
	cout<<"Please enter birth day: "<<endl;
	cin>>birthDay;
	familyMember->birthDay = birthDay;
	
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
		cout<<"\n 3 Display Nuclear Family "<<endl;
		cout<<"\n 4 Display Full Family "<<endl;
		cout<<"\n 9 Exit "<<endl;
		cin>>ch;
		switch(ch)
		{
			case 1:
				cout<<"Please enter the id of the targer family member:";
				cin>>id;
				if(id>=myFamily.getNextId()){
					cout<<"Error, Family member doesn't exist"<<endl;
					break;
				}
				
				cout<<"Please enter spouse's details: "<<endl;
				cin.ignore();
				temp = createFamilyMember(0);
				
				myFamily.addSpouse(id, temp);
				break;
			
			case 2:
				cout<<"Please enter the id of the targer family member:";
				cin>>id;
				if(id>=myFamily.getNextId()){
					cout<<"Error, Family member doesn't exist"<<endl;
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
					break;
				}
				
				myFamily.displayNuclearFamily(id);
				break;
				
			case 4:
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
