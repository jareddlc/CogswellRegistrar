#pragma once

ref class Worker {
	SQLiteCommand^  sCom;
	SQLiteConnection^  sCon;
	String ^file_audit;
	String ^file_master;
	TextBox ^outputBox;
	delegate void setTextBoxText(TextBox ^, String ^);
	setTextBoxText ^outputDelegate;
	
public:
	Worker(TextBox ^, String ^, String ^);
	void Work();
	void setTextBoxMethod(TextBox ^, String ^);
	void dropTables();
	void insertAudit();
	void insertMaster();
	void createLetterGrades();
	void createAudit();
	void createMaster();
	void createStanding();
	void createNeeds();
	void createCanTake();
	bool preReqs(String ^);
};
