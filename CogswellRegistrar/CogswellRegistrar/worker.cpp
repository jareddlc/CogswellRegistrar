#include "stdafx.h"
#include "worker.h"

Worker::Worker(TextBox ^textBox, String ^audit, String ^master)
{
	// excluded list
	array<String^>^ temp = {"MATH003","MATH110","MATH115"}; 
	excluded = temp;

	// textBox
	outputBox = textBox;
	outputDelegate = gcnew setTextBoxText(this, &Worker::setTextBoxMethod);
	outputBox->Invoke(outputDelegate, outputBox, "Initializing...");

	// input files
	this->file_audit = audit;
	this->file_master = master;

	// database queries
	this->dbQuery = (gcnew SQLiteCommand());
	this->dbQuery->CommandText = nullptr;

	// database connection
	this->dbConnection = (gcnew SQLiteConnection());
	this->dbConnection->ConnectionString = L"Data Source=Students.db";
	this->dbConnection->DefaultTimeout = 30;
	this->dbConnection->Flags = static_cast<SQLiteConnectionFlags>((SQLiteConnectionFlags::LogCallbackException | SQLiteConnectionFlags::LogModuleException));
	this->dbConnection->ParseViaFramework = false;

	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");
}

void Worker::setTextBoxMethod(System::Windows::Forms::TextBox ^T, String ^str)
{
	T->AppendText(str); 
}

void Worker::Work()
{
	outputBox->Invoke(outputDelegate, outputBox, "Connecting to Students.db...");
	dbConnection->Open();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

	outputBox->Invoke(outputDelegate, outputBox, "Dropping tables...");
	this->dropTables();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

	outputBox->Invoke(outputDelegate, outputBox, "Creating raw_audit table...");
	this->insertAudit();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");


	this->insertMaster();

	outputBox->Invoke(outputDelegate, outputBox, "Creating letterGrades table...");
	this->createLetterGrades();

	this->createAudit();

	this->createMaster();

	this->createStanding();

	this->createNeeds();

	this->createCanTake();

	outputBox->Invoke(outputDelegate, outputBox, "Closing Students.db...");
	dbConnection->Close();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

	outputBox->Invoke(outputDelegate, outputBox, "Process complete.\r\n");
}

void Worker::dropTables()
{
	dbQuery = dbConnection->CreateCommand();

	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS raw_master;", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS raw_audit;", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS letterGrades;", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS audit;", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS master;", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS standing;", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS needs;", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("DROP TABLE IF EXISTS canTake;", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("END", dbConnection);
	dbQuery->ExecuteNonQuery();
}

void Worker::createLetterGrades()
{
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE letterGrades (`letter` varchar(2) not null, `number` decimal(2,1) not null, primary key (`letter`, `number`));";
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery(); 
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('A+', '4.0');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('A', '4.0');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('A-', '3.7');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('B+', '3.3');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('B', '3.0');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('B-', '2.7');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('C+', '2.3');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('C', '2.0');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('C-', '1.7');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('D+', '1.3');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('D', '1.0');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('D-', '0.0');", dbConnection);
	dbQuery->ExecuteNonQuery();
	dbQuery = gcnew SQLiteCommand("INSERT INTO letterGrades VALUES('F', '0.0');", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("END", dbConnection);
	dbQuery->ExecuteNonQuery();
}

void Worker::insertAudit()
{
	int numRows = 0;
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE raw_audit (`studentInfo` text, `courseID` text, `courseDesc` text, `letterGrade` text, `completionStatus` text);";
	dbQuery->ExecuteNonQuery();
	//SQLite Defer for large insertion
	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery();
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
					dbQuery = gcnew SQLiteCommand("INSERT INTO raw_audit VALUES('"+temp[0]+"', '"+temp[1]+"', '"+temp[2]+"', '"+temp[5]+"', '"+temp[7]+"');", dbConnection);
					dbQuery->ExecuteNonQuery();
				}
			}
			//SQLite perform insertion
			dbQuery = gcnew SQLiteCommand("END", dbConnection);
			dbQuery->ExecuteNonQuery();
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
}

void Worker::insertMaster()
{
	int numRows = 0;
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE raw_master (`courseID` text, `credits` text, `prereq` text, `coreq` text);";
	dbQuery->ExecuteNonQuery();
	//SQLite Defer for large insertion
	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery(); 
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
					dbQuery = gcnew SQLiteCommand("INSERT INTO raw_master VALUES('"+temp[0]+"', '"+temp[1]+"', '"+temp[3]+"', '"+temp[4]+"');", dbConnection);
					dbQuery->ExecuteNonQuery();
				}
			}
			//SQLite perform insertion
			dbQuery = gcnew SQLiteCommand("END", dbConnection);
			dbQuery->ExecuteNonQuery();
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
}

void Worker::createAudit()
{
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE audit AS select `studentInfo` AS `studentID`, `courseID`, `completionStatus`, `number` AS `numericalGrade` FROM `raw_audit` left join `letterGrades` on `letterGrade` = `letter`	WHERE `courseID` != '';";
	dbQuery->ExecuteNonQuery();
}

void Worker::createMaster()
{
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE master AS select * FROM (select replace(`courseID`,' ', '') AS `courseID`, `prereq` FROM `raw_master`) ";
	dbQuery->ExecuteNonQuery();
}

void Worker::createStanding()
{
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE `standing` AS select distinct `studentID`, 'Senior' AS `collegeLevel` FROM `audit`";
	dbQuery->ExecuteNonQuery();
}

void Worker::createNeeds()
{
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE `needs` AS select `studentID`, `courseID` FROM `audit` WHERE `completionStatus` = 'R'";
	dbQuery->ExecuteNonQuery();
}

void Worker::createCanTake()
{
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE canTake ( `StudentID`  text, `CourseID` text)";
	dbQuery->ExecuteNonQuery();

	dbQuery->CommandText = "SELECT `studentID`, `courseID` FROM `needs`;";
	SQLiteDataReader ^reader = dbQuery->ExecuteReader();
	String ^row;

	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery(); 
	while(reader->Read())
    {
		for(int col = 0; col < reader->FieldCount; ++col)
        {
			row += reader->GetValue(col)->ToString();
			if(col == 0)
				row += ",";
        }
		if(this->preReqs(row))
		{
			array<String^> ^take;
			// take[0] = student ID, take[1] = course ID
			take = row->Split(',');
			dbQuery = gcnew SQLiteCommand("INSERT INTO canTake VALUES('"+take[0]+"', '"+take[1]+"');", dbConnection);
			dbQuery->ExecuteNonQuery();
		}
		row = "";
    }
	dbQuery = gcnew SQLiteCommand("END", dbConnection);
	dbQuery->ExecuteNonQuery();
}

bool Worker::preReqs(String ^pre)
{
	bool pass = false;
	array<String^> ^need;
	// need[0] = student ID, need[1] = course ID
	need = pre->Split(',');

	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "SELECT `prereq` FROM `master` WHERE `courseID` = \""+need[1]+"\";";
	SQLiteDataReader ^reader = dbQuery->ExecuteReader();
	String ^row;
	Regex^ regex = gcnew Regex("[A-Z]{1,4}[0-9]{3}[A-Z]{0,2}", RegexOptions::IgnoreCase);
	MatchCollection^ matches;
	String ^status;
	cliext::vector<String^> courses;
	cliext::vector<String^> formula;
	while(reader->Read())
    {
		for(int col = 0; col < reader->FieldCount; ++col)
        {
			row += reader->GetValue(col)->ToString();
			matches = regex->Matches(row);
			//outputBox->Invoke(outputDelegate, outputBox, need[1]+" needs: "+row+"\r\n");
			//outputBox->Invoke(outputDelegate, outputBox, matches->Count+"\r\n");
			for each (Match^ match in matches)
			{
				//outputBox->Invoke(outputDelegate, outputBox, match->ToString()+"\r\n");
				for each (String ^s in excluded)
				{
					if(s->Contains(match->ToString()))
					{
						//return true;
					}
				}				
				dbQuery = dbConnection->CreateCommand();
				dbQuery->CommandText = "SELECT `completionStatus` FROM `audit` WHERE `studentID` = \""+need[0]+"\" AND `courseID` = \""+match->ToString()+"\";";
				SQLiteDataReader ^lookup = dbQuery->ExecuteReader();
				if(lookup->HasRows == false)
				{
					//outputBox->Invoke(outputDelegate, outputBox, need[0]+"("+need[1]+"):"+match->ToString()+" Not found!\r\n");
				}
				while(lookup->Read())
				{
					//outputBox->Invoke(outputDelegate, outputBox, lookup->FieldCount+"\r\n");
					for(int i = 0; i < lookup->FieldCount; ++i)
					{
						status = lookup->GetValue(i)->ToString();
						//outputBox->Invoke(outputDelegate, outputBox, need[0]+"("+need[1]+"):"+match->ToString()+" = "+status+"\r\n");
						courses.push_back(match->ToString()+","+status);
						//return true;
					}
				}
			}
			// Do the logic
			formula = this->parsePreReq(row);

			int paren = 0;
			int j = 0;
			for(int i = 0; i < formula.size(); i++)
			{
				if(formula.at(i) == "(")
				{
					paren += 1;
				}
				if(formula.at(i) == ")")
				{
					paren -= 1;
				}
			}

			for(int i = 0; i < courses.size(); i++)
			{
				//outputBox->Invoke(outputDelegate, outputBox, courses.at(i)+" "+i+"\r\n");

				bool complete = false;
				array<String^> ^temp; // temp[0] = course, temp[1] = completion status
				temp = courses.at(i)->Split(',');
		
				// do we have atleast one pass course?
				if(temp[1] == "TC" || temp[1] == "C" || temp[1] == "E")
				{
					complete = true;
				}
		
				// last loop
				if(i == courses.size()-1)
				{
					if(complete == true)
					{
						//outputBox->Invoke(outputDelegate, outputBox, need[1]+" needs: "+row+"\r\n");
						//outputBox->Invoke(outputDelegate, outputBox, "we have one passed course\r\n");
						return true;
					}
					else
					{
						//outputBox->Invoke(outputDelegate, outputBox, need[1]+" failed: "+row+"\r\n");
						//outputBox->Invoke(outputDelegate, outputBox, "all the courses are not met\r\n");
						return false;
					}
				}
			}	
        }
		//outputBox->Invoke(outputDelegate, outputBox, need[1]+": "+reader->GetValue(0)->ToString()+"\r\n");
    } 
	
}

cliext::vector<String^> Worker::parsePreReq(String ^formula)
{
	String ^temp;
	cliext::vector<String^> stack;
	
	//outputBox->Invoke(outputDelegate, outputBox, "\r\n"+formula+" = FORMULA\r\n");
	for(int i = 0; i < formula->Length; i++)
	{
		// end of formula
		if(formula[i] == '(')
		{
			stack.push_back("(");
		}
		else if(formula[i] == ')')
		{
			stack.push_back(temp);
			stack.push_back(")");
			temp = "";
		}
		else if(formula[i] == ' ')
		{
			if(temp != "")
			{
				stack.push_back(temp);
			}
			//outputBox->Invoke(outputDelegate, outputBox, temp+"\r\n");
			temp = "";
		}
		else if(i == formula->Length-1)
		{
			temp += formula[i];
			stack.push_back(temp);
			//outputBox->Invoke(outputDelegate, outputBox, temp+"\r\n");
			temp = "";
		}
		else
		{
			temp += formula[i];
		}
	}

	return stack;
}
