#pragma once

namespace CogswellRegistrar {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Data::SQLite;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Text;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
		private: System::Windows::Forms::GroupBox^  group_settings;
		private: System::Windows::Forms::Button^  btn_audit;
		private: System::Windows::Forms::Button^  btn_master;
		private: System::Windows::Forms::Button^  btn_search;
		private: System::Windows::Forms::TextBox^  input_search;
		private: System::Windows::Forms::GroupBox^  group_table;
		public: OpenFileDialog^ audit_file;
		public: OpenFileDialog^ master_file;
		public: bool audit_sel;
		public: bool master_sel;
		private: System::Windows::Forms::Label^  label_master;
		private: System::Windows::Forms::Label^  label_audit;
		private: System::Windows::Forms::Button^  btn_run;
		private: System::Windows::Forms::TextBox^  textBox_status;
		public: System::Data::SQLite::SQLiteCommand^  sCom;
		public: System::Data::SQLite::SQLiteConnection^  sCon;
		private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->group_settings = (gcnew System::Windows::Forms::GroupBox());
			this->btn_run = (gcnew System::Windows::Forms::Button());
			this->label_master = (gcnew System::Windows::Forms::Label());
			this->label_audit = (gcnew System::Windows::Forms::Label());
			this->btn_search = (gcnew System::Windows::Forms::Button());
			this->input_search = (gcnew System::Windows::Forms::TextBox());
			this->btn_master = (gcnew System::Windows::Forms::Button());
			this->btn_audit = (gcnew System::Windows::Forms::Button());
			this->group_table = (gcnew System::Windows::Forms::GroupBox());
			this->textBox_status = (gcnew System::Windows::Forms::TextBox());
			this->sCom = (gcnew System::Data::SQLite::SQLiteCommand());
			this->sCon = (gcnew System::Data::SQLite::SQLiteConnection());
			this->group_settings->SuspendLayout();
			this->group_table->SuspendLayout();
			this->SuspendLayout();
			// 
			// group_settings
			// 
			this->group_settings->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->group_settings->Controls->Add(this->btn_run);
			this->group_settings->Controls->Add(this->label_master);
			this->group_settings->Controls->Add(this->label_audit);
			this->group_settings->Controls->Add(this->btn_search);
			this->group_settings->Controls->Add(this->input_search);
			this->group_settings->Controls->Add(this->btn_master);
			this->group_settings->Controls->Add(this->btn_audit);
			this->group_settings->Location = System::Drawing::Point(12, 12);
			this->group_settings->Name = L"group_settings";
			this->group_settings->Size = System::Drawing::Size(600, 82);
			this->group_settings->TabIndex = 0;
			this->group_settings->TabStop = false;
			this->group_settings->Text = L"Settings";
			// 
			// btn_run
			// 
			this->btn_run->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btn_run->Enabled = false;
			this->btn_run->Location = System::Drawing::Point(519, 48);
			this->btn_run->Name = L"btn_run";
			this->btn_run->Size = System::Drawing::Size(75, 23);
			this->btn_run->TabIndex = 6;
			this->btn_run->Text = L"Process";
			this->btn_run->UseVisualStyleBackColor = true;
			this->btn_run->Click += gcnew System::EventHandler(this, &Form1::btn_run_Click);
			// 
			// label_master
			// 
			this->label_master->AutoSize = true;
			this->label_master->Location = System::Drawing::Point(87, 53);
			this->label_master->Name = L"label_master";
			this->label_master->Size = System::Drawing::Size(62, 13);
			this->label_master->TabIndex = 5;
			this->label_master->Text = L"Select file...";
			// 
			// label_audit
			// 
			this->label_audit->AutoSize = true;
			this->label_audit->Location = System::Drawing::Point(87, 24);
			this->label_audit->Name = L"label_audit";
			this->label_audit->Size = System::Drawing::Size(62, 13);
			this->label_audit->TabIndex = 4;
			this->label_audit->Text = L"Select file...";
			// 
			// btn_search
			// 
			this->btn_search->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btn_search->Location = System::Drawing::Point(519, 19);
			this->btn_search->Name = L"btn_search";
			this->btn_search->Size = System::Drawing::Size(75, 23);
			this->btn_search->TabIndex = 3;
			this->btn_search->Text = L"Search";
			this->btn_search->UseVisualStyleBackColor = true;
			// 
			// input_search
			// 
			this->input_search->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->input_search->Location = System::Drawing::Point(345, 21);
			this->input_search->Name = L"input_search";
			this->input_search->Size = System::Drawing::Size(168, 20);
			this->input_search->TabIndex = 2;
			// 
			// btn_master
			// 
			this->btn_master->Location = System::Drawing::Point(6, 48);
			this->btn_master->Name = L"btn_master";
			this->btn_master->Size = System::Drawing::Size(75, 23);
			this->btn_master->TabIndex = 1;
			this->btn_master->Text = L"Master List";
			this->btn_master->UseVisualStyleBackColor = true;
			this->btn_master->Click += gcnew System::EventHandler(this, &Form1::btn_master_Click);
			// 
			// btn_audit
			// 
			this->btn_audit->Location = System::Drawing::Point(6, 19);
			this->btn_audit->Name = L"btn_audit";
			this->btn_audit->Size = System::Drawing::Size(75, 23);
			this->btn_audit->TabIndex = 0;
			this->btn_audit->Text = L"Audit List";
			this->btn_audit->UseVisualStyleBackColor = true;
			this->btn_audit->Click += gcnew System::EventHandler(this, &Form1::btn_audit_Click);
			// 
			// group_table
			// 
			this->group_table->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->group_table->Controls->Add(this->textBox_status);
			this->group_table->Location = System::Drawing::Point(12, 100);
			this->group_table->Name = L"group_table";
			this->group_table->Size = System::Drawing::Size(600, 330);
			this->group_table->TabIndex = 1;
			this->group_table->TabStop = false;
			this->group_table->Text = L"Table";
			// 
			// textBox_status
			// 
			this->textBox_status->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_status->Location = System::Drawing::Point(6, 19);
			this->textBox_status->Multiline = true;
			this->textBox_status->Name = L"textBox_status";
			this->textBox_status->ReadOnly = true;
			this->textBox_status->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->textBox_status->Size = System::Drawing::Size(588, 305);
			this->textBox_status->TabIndex = 0;
			// 
			// sCom
			// 
			this->sCom->CommandText = nullptr;
			// 
			// sCon
			// 
			this->sCon->ConnectionString = L"Data Source=Students.db";
			this->sCon->DefaultTimeout = 30;
			this->sCon->Flags = static_cast<System::Data::SQLite::SQLiteConnectionFlags>((System::Data::SQLite::SQLiteConnectionFlags::LogCallbackException | System::Data::SQLite::SQLiteConnectionFlags::LogModuleException));
			this->sCon->ParseViaFramework = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(624, 442);
			this->Controls->Add(this->group_table);
			this->Controls->Add(this->group_settings);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->Text = L"Cogswell Students";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->group_settings->ResumeLayout(false);
			this->group_settings->PerformLayout();
			this->group_table->ResumeLayout(false);
			this->group_table->PerformLayout();
			this->ResumeLayout(false);

		}
		#pragma endregion
		private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			this->textBox_status->Text += L"Application loaded.\r\n";
			this->textBox_status->Select(textBox_status->Text->Length,0);
			this->textBox_status->ScrollToCaret();
		}

		private: System::Void btn_audit_Click(System::Object^  sender, System::EventArgs^  e) {
			audit_file = gcnew OpenFileDialog();
			audit_file->Filter = "CSV Files|*.csv|All Files|*.*";
			if(audit_file->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			{
				return;
			}
			audit_sel = true;
			this->label_audit->Text = audit_file->FileName;
			this->textBox_status->Text += L"Audit file selected: "+audit_file->FileName+"\r\n";
			this->textBox_status->Select(textBox_status->Text->Length,0);
			this->textBox_status->ScrollToCaret();

			//Check to see if files where submitted
			if(audit_sel && master_sel) 
			{
				this->btn_run->Enabled = true;
			}
		}
		private: System::Void btn_master_Click(System::Object^  sender, System::EventArgs^  e) {
			master_file = gcnew OpenFileDialog();
			master_file->Filter = "CSV Files|*.csv|All Files|*.*";
			if(master_file->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			{
				return;
			}
			master_sel = true;
			this->label_master->Text = master_file->FileName;
			this->textBox_status->Text += L"Master file selected: "+master_file->FileName+"\r\n";
			this->textBox_status->Select(textBox_status->Text->Length,0);
			this->textBox_status->ScrollToCaret();

			//Check to see if files where submitted
			if(audit_sel && master_sel) 
			{
				this->btn_run->Enabled = true;
			}
		}
		private: System::Void btn_run_Click(System::Object^  sender, System::EventArgs^  e) {
			this->textBox_status->Text += L"Connecting to Students.db.\r\n";
			sCon->Open();
			this->textBox_status->Text += L"Connected to Students.db.\r\n";
			this->dropTables();
			this->createTables();
			this->csv_audit();

			this->textBox_status->Text += L"Closing Students.db.\r\n";
			sCon->Close();
			this->textBox_status->Text += L"Closed Students.db.\r\n";
		}
		private: System::Void dropTables() {
			this->textBox_status->Text += L"Dropping tables.\r\n";
			sCom = sCon->CreateCommand();
			sCom->CommandText = "DROP TABLE IF EXISTS `raw_master`;";
			sCom->ExecuteNonQuery();
			sCom->CommandText = "DROP TABLE IF EXISTS `raw_audit`;";
			sCom->ExecuteNonQuery();
			this->textBox_status->Text += L"Dropped tables.\r\n";
			this->textBox_status->Select(textBox_status->Text->Length,0);
			this->textBox_status->ScrollToCaret();
		}
		private: System::Void createTables() {
			this->textBox_status->Text += L"Creating tables.\r\n";
			sCom = sCon->CreateCommand();
			sCom->CommandText = "CREATE TABLE `raw_master` (`courseID` text,`title` text,`prereq` text,`coreq` text);";
			sCom->ExecuteNonQuery();
			sCom->CommandText = "create table `raw_audit` (`studentInfo` text,`courseID` text,`courseDesc` text,`letterGrade` text,`completionStatus` text);";
			sCom->ExecuteNonQuery();
			this->textBox_status->Text += L"Created tables.\r\n";
			this->textBox_status->Select(textBox_status->Text->Length,0);
			this->textBox_status->ScrollToCaret();
		}
		private: System::Void csv_audit() {
			int numRows = 0;
			//SQLite Defer for large insertion
			sCom = gcnew SQLiteCommand("BEGIN", sCon);
			sCom->ExecuteNonQuery(); 
			try 
			{
				StreamReader^ sr = gcnew StreamReader(audit_file->FileName);
				try	
				{
					String ^line;
					Array ^temp;
					while(line = sr->ReadLine())
					{
						numRows++;
						//temp = line->Split(',');
						sCom = gcnew SQLiteCommand("INSERT INTO raw_audit VALUES('0', '1', '2', '3', '4');", sCon);
						sCom->ExecuteNonQuery();
					}
					//SQLite perform insertion
					sCom = gcnew SQLiteCommand("END", sCon);
					sCom->ExecuteNonQuery();
					this->textBox_status->Text += L"Inserted rows = "+numRows+"\r\n";
				}
				finally
				{
					if(sr)
						delete(IDisposable^)sr;
				}
			}
			catch ( Exception^ e ) 
			{
				// Let the user know what went wrong.
				this->textBox_status->Text += L"The file could not be read:";
				this->textBox_status->Text += e->Message;
			}
		}

};
}
