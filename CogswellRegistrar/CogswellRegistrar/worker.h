#pragma once

ref class Worker {
	SQLiteCommand^  dbQuery;
	SQLiteConnection^  dbConnection;
	String^ file_audit;
	String^ file_master;
	TextBox^ outputBox;
	delegate void setTextBoxText(TextBox^, String^);
	setTextBoxText^ outputDelegate;
	cliext::vector<String^> init; // init vector becauase visual c++ is dumb
	
public:
	Worker(TextBox^, String^, String^);
	void Work();
	void setTextBoxMethod(TextBox^, String^);
	void dropTables();
	void createTables();
	void insertAudit();
	void insertMaster();
	void createCanTake();
	bool checkPrerequisite(String^);
	cliext::vector<String^> parseFormula(String^);
};
