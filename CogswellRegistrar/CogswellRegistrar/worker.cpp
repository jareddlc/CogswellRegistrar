#include "stdafx.h"
#include "worker.h"

Worker::Worker(TextBox ^textBox, String ^audit, String ^master)
{
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

	outputBox->Invoke(outputDelegate, outputBox, "Inserting data...");
	this->insertAudit();
	this->insertMaster();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

	outputBox->Invoke(outputDelegate, outputBox, "Creating tables...");
	this->createTables();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

	outputBox->Invoke(outputDelegate, outputBox, "Calculating...");
	this->createCanTake();
	outputBox->Invoke(outputDelegate, outputBox, "Done.\r\n");

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

void Worker::createTables()
{
	dbQuery = dbConnection->CreateCommand();

	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("CREATE TABLE `letterGrades` (`letter` varchar(2) not null, `number` decimal(2,1) not null, primary key (`letter`, `number`));", dbConnection);
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

	dbQuery = gcnew SQLiteCommand("CREATE TABLE `audit` AS select `studentInfo` AS `studentID`, `courseID`, `completionStatus`, `number` AS `numericalGrade` FROM `raw_audit` left join `letterGrades` on `letterGrade` = `letter`	WHERE `courseID` != '';", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("CREATE TABLE `master` AS select * FROM (select replace(`courseID`,' ', '') AS `courseID`, `prereq` FROM `raw_master`);", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("CREATE TABLE `standing` AS select distinct `studentID`, 'Senior' AS `collegeLevel` FROM `audit`;", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("CREATE TABLE `needs` AS select `studentID`, `courseID` FROM `audit` WHERE `completionStatus` = 'R';", dbConnection);
	dbQuery->ExecuteNonQuery();

	dbQuery = gcnew SQLiteCommand("END", dbConnection);
	dbQuery->ExecuteNonQuery();
}

void Worker::insertAudit()
{
	// vars
	int insertedRows = 0, currentRow = 0;
	Regex^ regexStudentId = gcnew Regex("^[A][0-9]{10}$", RegexOptions::IgnoreCase);
	Match^ match;

	// create table
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE `raw_audit` (`studentInfo` text, `courseID` text, `courseDesc` text, `letterGrade` text, `completionStatus` text);";
	dbQuery->ExecuteNonQuery();

	// create transaction for faster insertion
	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery();

	try 
	{
		StreamReader^ sr = gcnew StreamReader(this->file_audit);
		try	
		{
			String^ row;
			String^ id;
			array<String^>^ column;

			while(row = sr->ReadLine())
			{
				currentRow++;

				// split the columns (comma seperated file)
				column = row->Split(',');
				
				// look at first column for student ID
				match = regexStudentId->Match(column[0]);
				if(match->Success)
				{
					id = match->ToString();
					//outputBox->Invoke(outputDelegate, outputBox, id+"\r\n");
				}
				column[0] = id;
				// Insert the data. expected audit csv file should have 6 columns
				if(column->Length >= 6)
				{
					dbQuery = gcnew SQLiteCommand("INSERT INTO raw_audit VALUES('"+column[0]+"', '"+column[1]+"', '"+column[2]+"', '"+column[5]+"', '"+column[7]+"');", dbConnection);
					dbQuery->ExecuteNonQuery();
					insertedRows++;
				}
			}
			// close transaction and perform the queries
			dbQuery = gcnew SQLiteCommand("END", dbConnection);
			dbQuery->ExecuteNonQuery();
			outputBox->Invoke(outputDelegate, outputBox, "\r\nInserted into audit: "+insertedRows+" of "+currentRow+".\r\n");
		}
		finally
		{
			if(sr)
				delete(IDisposable^)sr;
		}
	}
	catch(Exception^ e) 
	{
		// Output the error
		outputBox->Invoke(outputDelegate, outputBox, "The file could not be read.\r\n");
		outputBox->Invoke(outputDelegate, outputBox, "Error at: "+currentRow+"\r\n");
		outputBox->Invoke(outputDelegate, outputBox, e->Message);
	}
}

void Worker::insertMaster()
{
	// vars
	int insertedRows = 0, currentRow = 0;

	// create table
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE `raw_master` (`courseID` text, `credits` text, `prereq` text, `coreq` text);";
	dbQuery->ExecuteNonQuery();

	// create transaction for faster insertion
	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery(); 
	try 
	{
		StreamReader^ sr = gcnew StreamReader(this->file_master);
		try	
		{
			String^ row;
			array<String^>^ column;

			while(row = sr->ReadLine())
			{
				currentRow++;

				// split the columns (comma seperated file)
				column = row->Split(',');

				// Insert the data. expected master csv file should have 14 columns
				if(column->Length >= 14)
				{
					dbQuery = gcnew SQLiteCommand("INSERT INTO raw_master VALUES('"+column[0]+"', '"+column[1]+"', '"+column[3]+"', '"+column[4]+"');", dbConnection);
					dbQuery->ExecuteNonQuery();
					insertedRows++;
				}
			}
			// close transaction and perform the queries
			dbQuery = gcnew SQLiteCommand("END", dbConnection);
			dbQuery->ExecuteNonQuery();
			outputBox->Invoke(outputDelegate, outputBox, "Inserted into master: "+insertedRows+" of "+currentRow+".\r\n");
		}
		finally
		{
			if(sr)
				delete(IDisposable^)sr;
		}
	}
	catch (Exception^ e) 
	{
		// Output the error
		outputBox->Invoke(outputDelegate, outputBox, "The file could not be read.\r\n");
		outputBox->Invoke(outputDelegate, outputBox, "Error at:"+currentRow+"\r\n");
		outputBox->Invoke(outputDelegate, outputBox, e->Message);
	}
}

void Worker::createCanTake()
{
	// create table
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "CREATE TABLE canTake ( `StudentID`  text, `CourseID` text)";
	dbQuery->ExecuteNonQuery();

	// select student and course from needs
	dbQuery->CommandText = "SELECT `studentID`, `courseID` FROM `needs`;";
	SQLiteDataReader^ reader = dbQuery->ExecuteReader();
	String^ row;

	// create transaction for faster insertion
	dbQuery = gcnew SQLiteCommand("BEGIN", dbConnection);
	dbQuery->ExecuteNonQuery();

	while(reader->Read())
    {
		// build the string StudentID,CourseID
		for(int col = 0; col < reader->FieldCount; ++col)
        {
			row += reader->GetValue(col)->ToString();
			if(col == 0)
				row += ",";
        }

		// check to see if student is able to take the course, if true insert
		if(this->checkPrerequisite(row))
		{
			// take[0] = student ID, take[1] = course ID
			array<String^>^ take;
			take = row->Split(',');

			dbQuery = gcnew SQLiteCommand("INSERT INTO canTake VALUES('"+take[0]+"', '"+take[1]+"');", dbConnection);
			dbQuery->ExecuteNonQuery();
		}
		row = "";
    }
	// close transaction and perform the queries
	dbQuery = gcnew SQLiteCommand("END", dbConnection);
	dbQuery->ExecuteNonQuery();
}

bool Worker::checkPrerequisite(String^ str)
{
	// vars
	bool requirement = false;
	String^ row;
	String^ status;
	cliext::vector<String^> courses;
	cliext::vector<String^> formula;
	Regex^ regexCourse = gcnew Regex("[A-Z]{1,4}[0-9]{3}[A-Z]{0,2}", RegexOptions::IgnoreCase);
	MatchCollection^ matches;

	// need[0] = student ID, need[1] = course ID
	array<String^>^ need;
	need = str->Split(',');

	// select prerequisites for the course
	dbQuery = dbConnection->CreateCommand();
	dbQuery->CommandText = "SELECT `prereq` FROM `master` WHERE `courseID` = \""+need[1]+"\";";

	SQLiteDataReader^ reader = dbQuery->ExecuteReader();
	
	while(reader->Read())
    {
		// grab the prereq string
		for(int col = 0; col < reader->FieldCount; ++col)
        {
			row += reader->GetValue(col)->ToString();
			outputBox->Invoke(outputDelegate, outputBox, need[1]+" needs: "+row+"\r\n");

			matches = regexCourse->Matches(row);
			//outputBox->Invoke(outputDelegate, outputBox, matches->Count+"\r\n");

			// for each course in prereq string
			for each (Match^ match in matches)
			{
				//outputBox->Invoke(outputDelegate, outputBox, match->ToString()+"\r\n");	

				// lookup the completetion status of the prereq course
				dbQuery = dbConnection->CreateCommand();
				dbQuery->CommandText = "SELECT `completionStatus` FROM `audit` WHERE `studentID` = \""+need[0]+"\" AND `courseID` = \""+match->ToString()+"\";";

				SQLiteDataReader^ lookup = dbQuery->ExecuteReader();

				// no completion found
				if(lookup->HasRows == false)
				{
					//outputBox->Invoke(outputDelegate, outputBox, need[0]+"("+need[1]+"):"+match->ToString()+" Not found!\r\n");
					courses.push_back(match->ToString()+",NONE");
				}

				// found the status
				while(lookup->Read())
				{
					//outputBox->Invoke(outputDelegate, outputBox, lookup->FieldCount+"\r\n");

					// push the status to the vector
					for(int i = 0; i < lookup->FieldCount; ++i)
					{
						status = lookup->GetValue(i)->ToString();
						courses.push_back(match->ToString()+","+status);
						//outputBox->Invoke(outputDelegate, outputBox, need[0]+"("+need[1]+"):"+match->ToString()+" = "+status+"\r\n");
					}
				}
			} // end of courses vector 

			// parse the forumula
			formula = this->parseFormula(row);

			// do the logic
			int paren = 0, and = 0, or = 0, j = 0;
			Stack^ stack = gcnew Stack;

			for(int i = 0; i < formula.size(); i++)
			{
				if(formula.at(i) == "(")
				{
					stack->Push("(");
					paren += 1;
				}
				else if(formula.at(i) == ")")
				{
					stack->Push(")");
					/*
					String^ peek = stack->Peek()->ToString();
					while(peek == "(")
					{
						outputBox->Invoke(outputDelegate, outputBox, "peek: "+peek+"\r\n");
						String^ pop = stack->Pop()->ToString();

						if(pop == "true")
						{
							outputBox->Invoke(outputDelegate, outputBox, "pop true\r\n");
						}
						else if(pop == "false")
						{
							outputBox->Invoke(outputDelegate, outputBox, "pop false\r\n");
						}
					}
					*/
					paren -= 1;

				}
				else if(formula.at(i) == "AND")
				{
					stack->Push("&");
					and += 1;
				}
				else if(formula.at(i) == "OR")
				{
					stack->Push("|");
					or += 1;
				}
				else if(formula.at(i) == "College")
				{
					// do nothing
				}
				else if(formula.at(i) == "Level=Junior")
				{
					// do nothing
				}
				else if(formula.at(i) == "Level=Senior")
				{
					// do nothing
				}
				else 
				{
					//outputBox->Invoke(outputDelegate, outputBox, "formula: "+formula.at(i)+" = ");
					if(courses.size() > j)
					{
						array<String^>^ course; // course[0] = course, course[1] = completion status
						course = courses.at(j)->Split(',');

						if(course[1] == "C" || course[1] == "TC" || course[1] == "E")
						{
							//requirement = true;
							stack->Push("true");
						}
						else
						{
							stack->Push("false");
						}
						
						outputBox->Invoke(outputDelegate, outputBox, courses.at(j)+"\r\n");
						j++;
					}
				}
				// else
				//outputBox->Invoke(outputDelegate, outputBox, formula.at(i)+" ");
			}
			//outputBox->Invoke(outputDelegate, outputBox, "\r\n");
			// for loop

			for(int k = 0; k = stack->Count; k ++)
			{
				outputBox->Invoke(outputDelegate, outputBox, stack->Pop()+" ");
			}
			outputBox->Invoke(outputDelegate, outputBox, "\r\n");
			return requirement;
        } 
		// while
		return requirement;
    }
	return requirement;
}

cliext::vector<String^> Worker::parseFormula(String ^formula)
{
	String^ buffer;
	cliext::vector<String^> stack;
	
	// iterate over entire formula
	for(int i = 0; i < formula->Length; i++)
	{
		if(formula[i] == '(')
		{
			stack.push_back("(");
		}
		else if(formula[i] == ')')
		{
			stack.push_back(buffer);
			stack.push_back(")");
			buffer = "";
		}
		else if(formula[i] == ' ')
		{
			if(buffer != "")
			{
				stack.push_back(buffer);
			}
			buffer = "";
		}
		else if(i == formula->Length-1)
		{
			buffer += formula[i];
			stack.push_back(buffer);
			buffer = "";
		}
		else
		{
			buffer += formula[i];
		}
	}

	return stack;
}
