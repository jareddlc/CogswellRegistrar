#pragma once

ref class Worker {
	SQLiteCommand^  dbQuery;
	SQLiteConnection^  dbConnection;
	String^ file_audit;
	String^ file_master;
	TextBox^ outputBox;
	delegate void setTextBoxText(TextBox^, String^);
	setTextBoxText^ outputDelegate;
	array<String^>^ excluded;
	cliext::vector<String^> init; // init vector becauase visual c++ is dumb
	
public:
	Worker(TextBox^, String^, String^);
	void Work();
	void setTextBoxMethod(TextBox^, String^);
	void dropTables();
	void insertAudit();
	void insertMaster();
	void createLetterGrades();
	void createAudit();
	void createMaster();
	void createStanding();
	void createNeeds();
	void createCanTake();
	bool preReqs(String^);
	cliext::vector<String^> parsePreReq(String^);
};
