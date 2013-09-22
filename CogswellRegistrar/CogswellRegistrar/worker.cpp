#include "stdafx.h"
#include "worker.h"

Worker::Worker(TextBox ^textBox, String ^audit, String ^master) {
	// textBox
	outputBox = textBox;
	outputDelegate = gcnew setTextBoxText(this, &Worker::setTextBoxMethod);
	outputBox->Invoke(outputDelegate, outputBox, "Initializing...");

	// file
	this->file_audit = audit;
	this->file_master = master;

	// sCom
	this->sCom = (gcnew SQLiteCommand());
	this->sCom->CommandText = nullptr;

	// sCon
	this->sCon = (gcnew SQLiteConnection());
	this->sCon->ConnectionString = L"Data Source=Students.db";
	this->sCon->DefaultTimeout = 30;
	this->sCon->Flags = static_cast<SQLiteConnectionFlags>((SQLiteConnectionFlags::LogCallbackException | SQLiteConnectionFlags::LogModuleException));
	this->sCon->ParseViaFramework = false;

	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::setTextBoxMethod(System::Windows::Forms::TextBox ^T, String ^str)
{
	T->AppendText(str); 
}

void Worker::Work()
{
	outputBox->Invoke(outputDelegate, outputBox, "Connecting to Students.db...");
	sCon->Open();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

	this->dropTables();
	this->insertAudit();
	this->insertMaster();
	this->createLetterGrades();
	this->createAudit();
	this->createMaster();
	this->createStanding();
	this->createNeeds();
	this->createCanTake();

	outputBox->Invoke(outputDelegate, outputBox, "Closing Students.db...");
	sCon->Close();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

	outputBox->Invoke(outputDelegate, outputBox, "Process complete.\r\n");
}

void Worker::dropTables()
{
	outputBox->Invoke(outputDelegate, outputBox, "Dropping tables...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "DROP TABLE IF EXISTS raw_master;";
	sCom->ExecuteNonQuery();
	sCom->CommandText = "DROP TABLE IF EXISTS raw_audit;";
	sCom->ExecuteNonQuery();
	sCom->CommandText = "DROP TABLE IF EXISTS letterGrades;";
	sCom->ExecuteNonQuery();
	sCom->CommandText = "DROP TABLE IF EXISTS audit;";
	sCom->ExecuteNonQuery();
	sCom->CommandText = "DROP TABLE IF EXISTS master;";
	sCom->ExecuteNonQuery();
	sCom->CommandText = "DROP TABLE IF EXISTS standing;";
	sCom->ExecuteNonQuery();
	sCom->CommandText = "DROP TABLE IF EXISTS needs;";
	sCom->ExecuteNonQuery();
	sCom->CommandText = "DROP TABLE IF EXISTS canTake;";
	sCom->ExecuteNonQuery();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::createLetterGrades()
{
	outputBox->Invoke(outputDelegate, outputBox, "Creating letterGrades table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE letterGrades (`letter` varchar(2) not null, `number` decimal(2,1) not null, primary key (`letter`, `number`));";
	sCom->ExecuteNonQuery();

	sCom = gcnew SQLiteCommand("BEGIN", sCon);
	sCom->ExecuteNonQuery(); 
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('A+', '4.0');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('A', '4.0');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('A-', '3.7');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('B+', '3.3');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('B', '3.0');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('B-', '2.7');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('C+', '2.3');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('C', '2.0');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('C-', '1.7');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('D+', '1.3');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('D', '1.0');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('D-', '0.0');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('F', '0.0');", sCon);
	sCom->ExecuteNonQuery();
	sCom = gcnew SQLiteCommand("END", sCon);
	sCom->ExecuteNonQuery();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::insertAudit()
{
	int numRows = 0;
	outputBox->Invoke(outputDelegate, outputBox, "Creating raw_audit table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE raw_audit (`studentInfo` text, `courseID` text, `courseDesc` text, `letterGrade` text, `completionStatus` text);";
	sCom->ExecuteNonQuery();
	//SQLite Defer for large insertion
	sCom = gcnew SQLiteCommand("BEGIN", sCon);
	sCom->ExecuteNonQuery();
	Regex^ regex = gcnew Regex("^[A][0-9]{10}$", RegexOptions::IgnoreCase);
	Match^ match;
	try 
	{
		StreamReader^ sr = gcnew StreamReader(this->file_audit);
		try	
		{
			String ^line;
			String ^id;
			array<String^>^ temp;
			while(line = sr->ReadLine())
			{
				numRows++;
				temp = line->Split(',');
				match = regex->Match(temp[0]);
				if(match->Success)
				{
					id = match->ToString();
					//outputBox->Invoke(outputDelegate, outputBox, id+"\r\n");
				}
				temp[0] = id;
				if(temp->Length >= 6)
				{
					sCom = gcnew SQLiteCommand("INSERT INTO raw_audit VALUES('"+temp[0]+"', '"+temp[1]+"', '"+temp[2]+"', '"+temp[5]+"', '"+temp[7]+"');", sCon);
					sCom->ExecuteNonQuery();
				}
			}
			//SQLite perform insertion
			sCom = gcnew SQLiteCommand("END", sCon);
			sCom->ExecuteNonQuery();
			outputBox->Invoke(outputDelegate, outputBox, "Inserted: "+numRows+". ");
		}
		finally
		{
			if(sr)
				delete(IDisposable^)sr;
		}
	}
	catch(Exception^ e) 
	{
		// Let the user know what went wrong.
		outputBox->Invoke(outputDelegate, outputBox, "The file could not be read.\r\n");
		outputBox->Invoke(outputDelegate, outputBox, "Error at:"+numRows+"\r\n");
		outputBox->Invoke(outputDelegate, outputBox, e->Message);
	}
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::insertMaster()
{
	int numRows = 0;
	outputBox->Invoke(outputDelegate, outputBox, "Creating raw_master table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE raw_master (`courseID` text, `credits` text, `prereq` text, `coreq` text);";
	sCom->ExecuteNonQuery();
	//SQLite Defer for large insertion
	sCom = gcnew SQLiteCommand("BEGIN", sCon);
	sCom->ExecuteNonQuery(); 
	try 
	{
		StreamReader^ sr = gcnew StreamReader(this->file_master);
		try	
		{
			String ^line;
			array<String^>^ temp;
			while(line = sr->ReadLine())
			{
				numRows++;
				temp = line->Split(',');
				if(temp->Length >= 14)
				{
					sCom = gcnew SQLiteCommand("INSERT INTO raw_master VALUES('"+temp[0]+"', '"+temp[1]+"', '"+temp[3]+"', '"+temp[4]+"');", sCon);
					sCom->ExecuteNonQuery();
				}
			}
			//SQLite perform insertion
			sCom = gcnew SQLiteCommand("END", sCon);
			sCom->ExecuteNonQuery();
			outputBox->Invoke(outputDelegate, outputBox, "Inserted: "+numRows+". ");
		}
		finally
		{
			if(sr)
				delete(IDisposable^)sr;
		}
	}
	catch (Exception^ e) 
	{
		// Let the user know what went wrong.
		outputBox->Invoke(outputDelegate, outputBox, "The file could not be read.\r\n");
		outputBox->Invoke(outputDelegate, outputBox, "Error at:"+numRows+"\r\n");
		outputBox->Invoke(outputDelegate, outputBox, e->Message);
	}
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::createAudit()
{
	outputBox->Invoke(outputDelegate, outputBox, "Creating audit table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE audit as select `studentInfo` as `studentID`, `courseID`, `completionStatus`, `number` as `numericalGrade` from `raw_audit` left join `letterGrades` on `letterGrade` = `letter`	where `courseID` != '';";
	sCom->ExecuteNonQuery();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::createMaster()
{
	outputBox->Invoke(outputDelegate, outputBox, "Creating master table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE master as select * from (select replace(`courseID`,' ', '') as `courseID`, `prereq` from `raw_master`) ";
	sCom->ExecuteNonQuery();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::createStanding()
{
	outputBox->Invoke(outputDelegate, outputBox, "Creating standing table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE `standing` as select distinct `studentID`, 'Senior' as `collegeLevel` from `audit`";
	sCom->ExecuteNonQuery();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::createNeeds()
{
	outputBox->Invoke(outputDelegate, outputBox, "Creating needs table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE `needs` as select `studentID`, `courseID` from `audit` where `completionStatus` = 'R'";
	sCom->ExecuteNonQuery();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::createCanTake()
{
	outputBox->Invoke(outputDelegate, outputBox, "Creating canTake table...");
	sCom = sCon->CreateCommand();
	sCom->CommandText = "CREATE TABLE CanTake ( `StudentID`  text, `CourseID` text)";
	sCom->ExecuteNonQuery();

	sCom->CommandText = "select `courseID`, `prereq` from `master`;";
	SQLiteDataReader ^reader = sCom->ExecuteReader();
	String ^row;
	array<String^>^ rows;
	while(reader->Read())
    {
		for (int col = 0; col < reader->FieldCount; ++col)
        {
			//outputBox->Invoke(outputDelegate, outputBox, reader->GetValue(col)->ToString());
			row += reader->GetValue(col)->ToString();
			if(col == 0)
				row += ",";
        }
		outputBox->Invoke(outputDelegate, outputBox, row+"\r\n");
		row = "";
    }
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

