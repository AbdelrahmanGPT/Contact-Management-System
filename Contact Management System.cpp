#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<cmath>
#include<string>
#include<array>
#include<algorithm>
#include<cctype>
#include<ctime>
#include<limits>
#include<vector>
#include<fstream>
#include<cstdio>
#include<regex>
#include"ABODAstd.h"

using namespace std;

string ContactsDataFileName = "CONTACTS.txt";
string SystemThemeColorsDataFileName = "SYSTEM THEME COLORS.txt";


struct SystemThemeColors
{
	CLIColors BackgroundColor = Black;
	CLIColors ForegroundColor = BrightWhite;
};

struct Contact {
	string Name;
	string PhoneNumber;
	string EmailAddress;
	string Company;
};

SystemThemeColors SystemThemeColor;

enum EditSettingsChoices {EditName = 1 , EditPhoneNumber = 2 , EditEmailAddress = 3 , 
						  EditCompany = 4};

enum ContactManagementSystemChoices {AddAContact = 1 , EditAContact = 2 ,
									FindAContact = 3 , DeleteAContact = 4 , 
									ShowContactList = 5 , ChangeSystemsTheme = 6 ,
								    ExitSystem = 7};

enum FindContactMenuChoices{FindByName = 1 , FindByPhoneNumber = 2 , 
	FindByEmailAddress = 3 , FindByCompany = 4};


enum ChangeSystemThemeMenuChoices{Background = 1 , Foreground = 2 , Default = 3};

void PrintContact(Contact& Contact)
{
	cout << "\nTHE FOLLOWING ARE THE CONTACT DETAILS : \n";
	cout << "--------------------------------------------------------\n";
	cout << "NAME           : " << Contact.Name << "\n";
	cout << "PHONE NUMBER   : " << Contact.PhoneNumber << "\n";
	cout << "E-MAIL ADDRESS : " << Contact.EmailAddress << "\n";
	cout << "COMPANY        : " << Contact.Company << "\n";
	cout << "--------------------------------------------------------\n\n\n";
}

void SaveSystemThemeColorsToFile();

void SaveContactsDataToFile(vector<Contact>& Contacts);

bool IsPhoneKey(string PhoneKey)
{
	for (int i = 0; i < PhoneKey.length(); i++)
	{
		if (PhoneKey[0] != '+')
		{
			printf("\nINVALID PHONE KEY , YOU HAVEN'T ENTERED + SIGN CHARACTER IN THE BEGINNING OF KEY !\n");
			return 0;
		}

		if (i != 0 && isdigit(PhoneKey[i]) == false)
		{
			printf("\nINVALID PHONE KEY , YOU HAVE ENTERED NON-DIGIT CHARACTER !\n");
			return 0;
		}
	}

	return 1;
}

bool IsPhoneNumber(string PhoneNumber)
{
	for (int i = 0; i < PhoneNumber.size(); i++)
	{
		if (!isdigit(PhoneNumber[i]))
		{
			printf("\nINVALID PHONE NUMBER , YOU HAVE ENTERED NON-DIGIT CHARACTER !\n");
			return 0;
		}
	}

	return 1;
}

string ReadPhoneNumber(string Message)
{
	string PhoneNumber;
	do
	{

		PhoneNumber = ReadStringLine(Message);

	} while (!IsPhoneNumber(PhoneNumber));

	return PhoneNumber;
}

string ReadPhoneKey(string Message)
{
	string PhoneKey;
	do
	{

		PhoneKey = ReadStringLine(Message);

	} while (!IsPhoneKey(PhoneKey));

	return PhoneKey;
}

bool IsEmailAddressCorrect(string EmailAddress)
{
		const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

		if (!regex_match(EmailAddress, pattern))
		{
			printf("\nINVALID EMAIL ADDRESS FORMULA !\n");
			return 0;
		}

		return 1;
}

string ReadContactEmailAddress(string Message)
{
	string EmailAddress;
	do
	{
		EmailAddress = ReadStringLine(Message);

	} while (!IsEmailAddressCorrect(EmailAddress));

	return EmailAddress;
}

string ReadContactFullPhoneNumber(string PhoneKeyMessage, string PhoneNumberMessage)
{
	string PhoneNumber, PhoneKey;

	PhoneKey = ReadPhoneKey(PhoneKeyMessage);
	PhoneNumber = ReadPhoneNumber(PhoneNumberMessage);
	
	return PhoneKey + PhoneNumber;
}

Contact ReadNewContact()
{
	Contact Contact;

	printf("\nADDING NEW CONTACT : \n");

	Contact.Name = ReadStringLine("ENTER CONTACT NAME");
	Contact.PhoneNumber = ReadContactFullPhoneNumber("ENTER PHONE KEY", "ENTER PHONE NUMBER");
	Contact.EmailAddress = ReadContactEmailAddress("ENTER CONTACT EMAIL ADDRESS");
	Contact.Company = ReadStringLine("ENTER CONTACT COMPANY");

	return Contact;
}

int FindContactByNameAndReturnIndex(string ContactName, vector<Contact>& Contacts)
{
	for (int ContactIndex = 0; ContactIndex < Contacts.size(); ContactIndex++)
	{
		if (Contacts[ContactIndex].Name == ContactName)
		{
			return ContactIndex;
		}
	}

	printf("\nCONTACT WITH NAME [%s] IS NOT FOUND!\n", ContactName.c_str());
	return -1;
}

void AddNewContact(vector<Contact>& Contacts)
{
	SectionBanner("ADD NEW CONTACT SCREEN" , 2 , "=");

	do
	{
		Contact Contact = ReadNewContact();

		if (DoYouWantToDoThisWithoutClear("ARE YOU SURE YOU WANT TO ADD THIS CONTACT"))
		{
			Contacts.push_back(Contact);
			SaveContactsDataToFile(Contacts);
			printf("\nTHE CONTACT IS ADDED SUCCESSFULLY !\n");
		}

	} while (DoYouWantToDoThisWithoutClear("DO YOU WANT TO ADD ANOTHER CONTACT"));

	//GoToPauseMessage("\n\nPRESS ANY KEY TO GO BACK TO CONTACT MANAGEMENT MENU...");
}

void PrintEditMenu()
{
	cout << "\n=====================================\n\n";
	cout << Tab(1) << "EDIT SETTINGS\n\n";
	cout << "=====================================\n";
	cout << Tab(1) << "[1] EDIT NAME." << "\n";
	cout << Tab(1) << "[2] EDIT PHONE NUMBER." << "\n";
	cout << Tab(1) << "[3] EDIT E-MAIL ADDRESS." << "\n";
	cout << Tab(1) << "[4] EDIT COMPANY." << "\n";
	cout << "=====================================\n\n";
}

EditSettingsChoices ReadEditSettingsChoice()
{
	return (EditSettingsChoices)(ValidateNumberInRange(1, 4, "CHOOSE WHAT DO YOU WANT TO DO ? [1 TO 4] ?"));
}

void PerformEditSettingsChoice(Contact& Contact, EditSettingsChoices EditSettingsChoice)
{
	switch (EditSettingsChoice)
	{
	case EditName:
		Contact.Name = ReadStringLine("ENTER NEW CONTACT NAME");
		break;
	case EditPhoneNumber:
		Contact.PhoneNumber = ReadContactFullPhoneNumber("ENTER NEW PHONE KEY", "ENTER NEW PHONE NUMBER");
		break;
	case EditEmailAddress:
		Contact.EmailAddress = ReadContactEmailAddress("ENTER NEW E-MAIL ADDRESS");
		break;
	case EditCompany:
		Contact.Company = ReadStringLine("ENTER NEW COMPANY NAME");
		break;
	default:
		break;
	}
}

void PerformContactsEdits(Contact& Contact)
{
	do
	{
		PrintEditMenu();
		EditSettingsChoices EditSettingsChoice = ReadEditSettingsChoice();
		PerformEditSettingsChoice(Contact, EditSettingsChoice);

	} while (DoYouWantToDoThisWithoutClear("DO YOU WANT TO EDIT ANOTHER INFORMATION IN THIS CONTACT ?"));
}

void EditContact(vector<Contact>& Contacts)
{

	do
	{
		SectionBanner("EDIT CONTACT SCREEN", 2, "=");

		string ContactName = ReadStringLine("ENTER CONTACT NAME");
		int ContactIndex = FindContactByNameAndReturnIndex(ContactName, Contacts);

		if (ContactIndex != -1)
		{
			PrintContact(Contacts[ContactIndex]);

			if (DoYouWantToDoThisWithoutClear("ARE YOU SURE YOU WANT TO EDIT THIS CONTACT"))
			{
				PerformContactsEdits(Contacts[ContactIndex]);
				SaveContactsDataToFile(Contacts);
				printf("\nCONTACT IS EDITTED SUCCESSFULLY\n");
			}
		}

	} while (DoYouWantToDoThisWithClear("DO YOU WANT TO EDIT ANOTHER CONTACT ?"));
}

void PrintFindContactMenu()
{
	cout << "=====================================================================\n\n";
	cout << Tab(3) << "FIND CONTACT MENU\n\n";
	cout << "=====================================================================\n";
	cout << Tab(3) << "[1] FIND CONTACT BY NAME." << "\n";
	cout << Tab(3) << "[2] FIND CONTACT BY PHONE NUMBER." << "\n";
	cout << Tab(3) << "[3] FIND CONTACT BY E-MAIL ADDRESS." << "\n";
	cout << Tab(3) << "[4] FIND CONTACT BY COMPANY." << "\n";
	cout << "=====================================================================\n\n";

}

FindContactMenuChoices ReadFindContactMenuChoice()
{
	return (FindContactMenuChoices)(ValidateDoubleNumberInRange(1, 4, "CHOOSE WHAT DO YOU WANT TO DO ? [1 TO 4] ? "));
}

void PrintContactsTableHeader()
{
	PrintLineWithNewLines("_", 118);
	PrintStringElementInTable("NO.", 4);
	PrintStringElementInTable("NAME", 30);
	PrintStringElementInTable("PHONE NUMBER", 20);
	PrintStringElementInTable("E-MAIL ADDRESS", 38);
	PrintStringElementInTable("COMPANY", 15);
	PrintLineWithNewLines("_", 118);
}

void PrintContactLineInTable(Contact& Contact, int& ContactNumber)
{
	
	PrintNumberElementInTable(++ContactNumber, 4);
	PrintStringElementInTable(Contact.Name, 30);
	PrintStringElementInTable(Contact.PhoneNumber, 20);
	PrintStringElementInTable(Contact.EmailAddress, 38);
	PrintStringElementInTable(Contact.Company, 15);
	printf("\n");
	
}

void FindContactByName(vector<Contact>&Contacts)
{
	string ContactName = ReadStringLine("ENTER CONTACT NAME");
	int SearchResults = 0;

	cout << "\n\n";
	cout << Tab(5) << "SEARCH RESULT(S) FOR CONTACT NAME (" << ContactName << ")";

	PrintContactsTableHeader();

	for (Contact& Contact : Contacts)
	{
		if (LowerCaseAllString(Contact.Name) == LowerCaseAllString(ContactName) || LowerCaseAllString(Contact.Name).find(LowerCaseAllString(ContactName)) != std::string::npos)
		{
			
			PrintContactLineInTable(Contact , SearchResults);
		}
	}

	PrintLineWithNewLines("_", 118);

	printf("\n%s%d SEARCH RESULT(S) ARE FOUND\n\n" , Tab(5).c_str(), SearchResults);
}

void FindContactByPhoneNumber(vector<Contact>& Contacts)
{
	string PhoneNumber = ReadStringLine("ENTER PHONE NUMBER");
	int SearchResults = 0;

	cout << "\n\n";
	cout << Tab(5) << "SEARCH RESULT(S) FOR CONTACT PHONE NUMBER (" << PhoneNumber << ")";

	PrintContactsTableHeader();

	for (Contact& Contact : Contacts)
	{
		if (Contact.PhoneNumber == PhoneNumber || Contact.PhoneNumber.find(PhoneNumber) != string::npos)
		{
			PrintContactLineInTable(Contact, SearchResults);
		}
	}

	PrintLineWithNewLines("_", 118);

	printf("\n%s%d SEARCH RESULT(S) ARE FOUND\n\n", Tab(5).c_str(), SearchResults);

}

void FindContactByEmailAddress(vector<Contact>& Contacts)
{
	string EmailAddress = ReadStringLine("ENTER CONTACT E-MAIL ADDRESS");
	int SearchResults = 0;

	cout << "\n\n";
	cout << Tab(5) << "SEARCH RESULT(S) FOR CONTACT E-MAIL ADDRESS (" << EmailAddress << ")";

	PrintContactsTableHeader();

	for (Contact& Contact : Contacts)
	{
		if (LowerCaseAllString(Contact.EmailAddress) == LowerCaseAllString(EmailAddress) || LowerCaseAllString(Contact.EmailAddress).find(LowerCaseAllString(EmailAddress)) != string::npos)
		{
			PrintContactLineInTable(Contact, SearchResults);
		}
	}

	PrintLineWithNewLines("_", 118);

	printf("\n%s%d SEARCH RESULT(S) ARE FOUND\n\n", Tab(5).c_str(), SearchResults);

}

void FindContactByCompany(vector<Contact>& Contacts)
{
	string ContactCompany = ReadStringLine("ENTER CONTACT COMPANY");
	int SearchResults = 0;

	cout << "\n\n";
	cout << Tab(5) << "SEARCH RESULT(S) FOR CONTACT COMPANY (" << ContactCompany << ")";

	PrintContactsTableHeader();

	for (Contact& Contact : Contacts)
	{
		if (LowerCaseAllString(Contact.Company) == LowerCaseAllString(ContactCompany) || LowerCaseAllString(Contact.Company).find(LowerCaseAllString(ContactCompany)) != std::string::npos)
		{
			PrintContactLineInTable(Contact, SearchResults);
		}
	}

	PrintLineWithNewLines("_", 118);

	printf("\n%s%d SEARCH RESULT(S) ARE FOUND\n\n", Tab(5).c_str(), SearchResults);

}

void PerformFindContactMenuChoice(FindContactMenuChoices FindContactMenuChoice, vector<Contact>& Contacts)
{
	switch (FindContactMenuChoice)
	{
	case FindByName:
		FindContactByName(Contacts);
		break;
	case FindByPhoneNumber:
		FindContactByPhoneNumber(Contacts);
		break;
	case FindByEmailAddress:
		FindContactByEmailAddress(Contacts);
		break;
	case FindByCompany:
		FindContactByCompany(Contacts);
		break;
	}
}

void FindContact(vector<Contact>& Contacts)
{
	do
	{
		PrintFindContactMenu();
		FindContactMenuChoices FindContactMenuChoice = ReadFindContactMenuChoice();
		PerformFindContactMenuChoice(FindContactMenuChoice, Contacts);

	} while (DoYouWantToDoThisWithClear("DO YOU WANT TO FIND ANOTHER CONTACT ?"));
}

void DeleteContact(vector<Contact>& Contacts)
{
	SectionBanner("DELETE CONTACT SCREEN", 2, "=");

	do
	{

		string ContactName = ReadStringLine("PLEASE ENTER CONTACT NAME");
		int ContactIndex = FindContactByNameAndReturnIndex(ContactName, Contacts);

		if (ContactIndex != -1)
		{
			PrintContact(Contacts[ContactIndex]);

			if (DoYouWantToDoThisWithoutClear("ARE YOU SURE YOU WANT TO DELETE THIS ACCOUNT ?"))
			{
				Contacts.erase(Contacts.begin() + ContactIndex);
				SaveContactsDataToFile(Contacts);
				printf("\nCONTACT IS DELETED SUCCESSFULLY\n");
			}
		}
	} while (DoYouWantToDoThisWithoutClear("DO YOU WANT TO DELETE ANOTHER CONTACT ?"));
}

void PrintContactList(vector<Contact>& Contacts)
{
	cout << "\n\n";
	cout << Tab(5) << "CONTACTS LIST (" << Contacts.size() << ") CONTACT(S).";

	PrintContactsTableHeader();

	int ContactNumber = 0;
	for (Contact& Contact : Contacts)
	{
		PrintContactLineInTable(Contact, ContactNumber);
	}

	PrintLineWithNewLines("_", 118);
}

void PrintChangeSystemThemeMenu()
{
	cout << "=====================================================================\n\n";
	cout << Tab(3) << "CHANGE SYSTEM THEME MENU\n\n";
	cout << "=====================================================================\n";
	cout << Tab(3) << "[1] CHANGE BACKGROUND COLOR." << "\n";
	cout << Tab(3) << "[2] CHANGE FOREGROUND COLOR [TEXT COLOR]." << "\n";
	cout << Tab(3) << "[3] RETURN DEFAULT SYSTEM THEME." << "\n";
	cout << "=====================================================================\n\n";
}

ChangeSystemThemeMenuChoices ReadChangeSystemThemeMenuChoice()
{
	return (ChangeSystemThemeMenuChoices)(ValidateNumberInRange(1, 3, "CHOOSE WHAT DO YOU WANT TO DO ? [1 TO 3] ? "));
}

void PrintColorsMenu()
{
	cout << "====================================================================\n\n";
	cout << Tab(3) << "COLORS MENU\n\n";
	cout << "====================================================================\n";
	cout << Tab(2) << "[1] BLACK";
	cout << Tab(2) << "[2] BLUE" << "\n";
	cout << Tab(2) << "[3] GREEN";
	cout << Tab(2) << "[4] AQUA" << "\n";
	cout << Tab(2) << "[5] RED";
	cout << Tab(3) << "[6] PURPLE" << "\n";
	cout << Tab(2) << "[7] YELLOW";
	cout << Tab(2) << "[8] WHITE" << "\n";
	cout << Tab(2) << "[9] GRAY" ;
	cout << Tab(2) << "[10] LIGHT BLUE" << "\n";
	cout << Tab(2) << "[11] LIGHT GREEN" ;
	cout << Tab(1) << "[12] LIGHT AQUA" << "\n";
	cout << Tab(2) << "[13] LIGHT RED" ;
	cout << Tab(2) << "[14] LIGHT PURPLE" << "\n";
	cout << Tab(2) << "[15] LIGHT YELLOW";
	cout << Tab(1) << "[16] BRIGHT WHITE" << "\n";
	cout << "====================================================================\n\n";
}

CLIColors ReadColorsMenuChoice()
{
	return (CLIColors)(ValidateNumberInRange(1, 16, "CHOOSE THE COLOR YOU WANT ? [1 TO 16] ? ") - 1);
}

string ReturnChangeSystemThemeMenuChoiceString(ChangeSystemThemeMenuChoices ChangeSystemThemeMenuChoice)
{
	string ChoiceString[]{ "" , "BACKGROUND" , "FOREGROUND" };

	return ChoiceString[ChangeSystemThemeMenuChoice];
}

void ChangeSystemTheme(SystemThemeColors &SystemThemeColor)
{
	do
	{
		PrintChangeSystemThemeMenu();
		ChangeSystemThemeMenuChoices ChangeSystemThemeMenuChoice = ReadChangeSystemThemeMenuChoice();

		if (ChangeSystemThemeMenuChoice == Default)
		{
			ChangeCLIColors(SystemThemeColor.BackgroundColor = Black, SystemThemeColor.ForegroundColor = BrightWhite);
			SaveSystemThemeColorsToFile();
		}
		
		else
		{
			do
			{
				PrintColorsMenu();
				CLIColors Color = ReadColorsMenuChoice();

				ChangeSystemThemeMenuChoice == Background ? ChangeCLIColors(SystemThemeColor.BackgroundColor = Color, SystemThemeColor.ForegroundColor) : ChangeCLIColors(SystemThemeColor.BackgroundColor, SystemThemeColor.ForegroundColor = Color);
				SaveSystemThemeColorsToFile();

			} while (DoYouWantToDoThisWithoutClear("DO YOU WANT TO CHANGE ANOTHER COLOR IN " + ReturnChangeSystemThemeMenuChoiceString(ChangeSystemThemeMenuChoice) + " ?"));
		}
		
	} while (DoYouWantToDoThisWithClear("DO YOU WANT TO CHANGE ANOTHER COLOR IN SYSTEM THEME?"));
}

string SystemThemeColorsToRecord(SystemThemeColors SystemThemeColors , string Separator)
{
	return to_string(SystemThemeColors.BackgroundColor) + Separator + to_string(SystemThemeColors.ForegroundColor);
}

SystemThemeColors RecordToSystemThemeColors(string Record, string Separator)
{
	vector<string>SystemThemeColorsVector = SpiltStringWordsToVector(Record, Separator);

	SystemThemeColors SystemThemeColors;
	SystemThemeColors.BackgroundColor = (CLIColors)(stoi(SystemThemeColorsVector[0]));
	SystemThemeColors.ForegroundColor = (CLIColors)(stoi(SystemThemeColorsVector[1]));

	return SystemThemeColors;
}

void SaveSystemThemeColorsToFile()
{
	fstream File;

	File.open(SystemThemeColorsDataFileName, ios::out);

	if (File.is_open())
	{
		File << SystemThemeColorsToRecord(SystemThemeColor, "#//#");

		File.close();
	}

}

void LoadSystemThemeColorsFromFile()
{
	fstream File;

	File.open(SystemThemeColorsDataFileName, ios::in);

	if (File.is_open())
	{
		string Record;
		File >> Record;
		SystemThemeColor = RecordToSystemThemeColors(Record, "#//#");
	}
}

string ContactToRecord(Contact Contact , string Separator)
{
	return Contact.Name + Separator + Contact.PhoneNumber + Separator + Contact.EmailAddress + Separator + Contact.Company;
}

Contact RecordToContact(string Record, string Separator)
{
	vector<string> ContactVector = SpiltStringWordsToVector(Record, Separator);
	Contact Contact;

	Contact.Name = ContactVector[0];
	Contact.PhoneNumber = ContactVector[1];
	Contact.EmailAddress = ContactVector[2];
	Contact.Company = ContactVector[3];

	return Contact;
}

void SaveContactsDataToFile(vector<Contact>& Contacts)
{
	fstream File;
	File.open(ContactsDataFileName, ios::out);

	if (File.is_open())
	{
		for (Contact& Contact : Contacts)
		{
			File << ContactToRecord(Contact, "#//#") << "\n";
		}
		File.close();
	}
}

void LoadContactsDataFromFile(vector<Contact>& Contacts)
{
	fstream File;
	File.open(ContactsDataFileName, ios::in);

	if (File.is_open())
	{
		string Record;
		while (getline(File, Record))
		{
			Contacts.push_back(RecordToContact(Record, "#//#"));
		}

		File.close();
	}
}

void PrintContactManagementSystemMenu()
{
	cout << "====================================================================\n\n";
	cout << Tab(3) << "CONTACT MANAGEMENT SYSTEM MENU\n\n";
	cout << "====================================================================\n";
	cout << Tab(3) << "[1] ADD A NEW CONTACT." << "\n";
	cout << Tab(3) << "[2] EDIT A CONTACT." << "\n";
	cout << Tab(3) << "[3] FIND A CONTACT." << "\n";
	cout << Tab(3) << "[4] DELETE A CONTACT." << "\n";
	cout << Tab(3) << "[5] SHOW CONTACTS LIST." << "\n";
	cout << Tab(3) << "[6] CHANGE SYSTEM THEME." << "\n";
	cout << Tab(3) << "[7] EXIT CONTACT MANAGEMENT SYSTEM." << "\n";
	cout << "====================================================================\n\n";
}

ContactManagementSystemChoices ReadContactManagementSystemChoice()
{
	return (ContactManagementSystemChoices)(ValidateNumberInRange(1, 7, "CHOOSE WHAT DO YOU WANT TO DO ? [1 TO 7] ?"));
}

void PerformContactManagementSystemChoice(ContactManagementSystemChoices ContactManagementSystemChoice , vector<Contact>&Contacts)
{
	switch (ContactManagementSystemChoice)
	{
	case AddAContact:
		ClearScreen();
		AddNewContact(Contacts);
		SaveContactsDataToFile(Contacts);
		GoToPauseMessage("\n\nPRESS ANY KEY TO GO BACK TO CONTACT MANAGEMENT SYSTEM MENU...");
		break;

	case EditAContact:
		ClearScreen();
		EditContact(Contacts);
		SaveContactsDataToFile(Contacts);
		GoToPauseMessage("\n\nPRESS ANY KEY TO GO BACK TO CONTACT MANAGEMENT SYSTEM MENU...");
		break;

	case FindAContact :
		ClearScreen();
		FindContact(Contacts);
		GoToPauseMessage("\n\nPRESS ANY KEY TO GO BACK TO CONTACT MANAGEMENT SYSTEM MENU...");
		break;

	case DeleteAContact:
		ClearScreen();
		DeleteContact(Contacts);
		SaveContactsDataToFile(Contacts);
		GoToPauseMessage("\n\nPRESS ANY KEY TO GO BACK TO CONTACT MANAGEMENT SYSTEM MENU...");
		break;

	case ShowContactList:
		ClearScreen();
		PrintContactList(Contacts);
		GoToPauseMessage("\n\nPRESS ANY KEY TO GO BACK TO CONTACT MANAGEMENT SYSTEM MENU...");
		break;

	case ChangeSystemsTheme:
		ClearScreen();
		ChangeSystemTheme(SystemThemeColor);
		GoToPauseMessage("\n\nPRESS ANY KEY TO GO BACK TO CONTACT MANAGEMENT SYSTEM MENU...");
		break;

	case ExitSystem:
		SaveContactsDataToFile(Contacts);
		ExitProgram("GOOD BYE , CONTACT MANAGEMENT SYSTEM :-)", 2, "=" , 75 , SystemThemeColor.BackgroundColor , SystemThemeColor.ForegroundColor);
		break;
	}
}

void ContactManagementSystem()
{
	LoadSystemThemeColorsFromFile();
	ChangeCLIColors(SystemThemeColor.BackgroundColor, SystemThemeColor.ForegroundColor);
	vector<Contact>Contacts;
	LoadContactsDataFromFile(Contacts);
	StartProgram("CONTACT MANAGEMENT SYSTEM", 2, "=" , 60, SystemThemeColor.BackgroundColor, SystemThemeColor.ForegroundColor);

	while (true)
	{
		ClearScreen();
		PrintContactManagementSystemMenu();
		ContactManagementSystemChoices ContactManagementSystemChoice = ReadContactManagementSystemChoice();
		PerformContactManagementSystemChoice(ContactManagementSystemChoice, Contacts);
		
	}

}

int main()
{
	ContactManagementSystem();

	return 0;
}