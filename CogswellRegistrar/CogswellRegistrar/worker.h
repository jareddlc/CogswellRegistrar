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
	cliext::vector<String^> init; // init vector to avoid error LNK2022
	
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
