#pragma once

namespace CogswellRegistrar {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
	protected: 
	private: System::Windows::Forms::Button^  btn_audit;
	private: System::Windows::Forms::Button^  btn_master;
	private: System::Windows::Forms::Button^  btn_search;
	private: System::Windows::Forms::TextBox^  input_search;
	private: System::Windows::Forms::GroupBox^  group_table;

	protected: 
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
			this->btn_search = (gcnew System::Windows::Forms::Button());
			this->input_search = (gcnew System::Windows::Forms::TextBox());
			this->btn_master = (gcnew System::Windows::Forms::Button());
			this->btn_audit = (gcnew System::Windows::Forms::Button());
			this->group_table = (gcnew System::Windows::Forms::GroupBox());
			this->group_settings->SuspendLayout();
			this->SuspendLayout();
			// 
			// group_settings
			// 
			this->group_settings->AutoSize = true;
			this->group_settings->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->group_settings->Controls->Add(this->btn_search);
			this->group_settings->Controls->Add(this->input_search);
			this->group_settings->Controls->Add(this->btn_master);
			this->group_settings->Controls->Add(this->btn_audit);
			this->group_settings->Location = System::Drawing::Point(12, 12);
			this->group_settings->Name = L"group_settings";
			this->group_settings->Size = System::Drawing::Size(600, 90);
			this->group_settings->TabIndex = 0;
			this->group_settings->TabStop = false;
			this->group_settings->Text = L"Settings";
			// 
			// btn_search
			// 
			this->btn_search->Location = System::Drawing::Point(519, 19);
			this->btn_search->Name = L"btn_search";
			this->btn_search->Size = System::Drawing::Size(75, 23);
			this->btn_search->TabIndex = 3;
			this->btn_search->Text = L"Search";
			this->btn_search->UseVisualStyleBackColor = true;
			// 
			// input_search
			// 
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
			// 
			// btn_audit
			// 
			this->btn_audit->Location = System::Drawing::Point(6, 19);
			this->btn_audit->Name = L"btn_audit";
			this->btn_audit->Size = System::Drawing::Size(75, 23);
			this->btn_audit->TabIndex = 0;
			this->btn_audit->Text = L"Audit List";
			this->btn_audit->UseVisualStyleBackColor = true;
			// 
			// group_table
			// 
			this->group_table->Location = System::Drawing::Point(12, 109);
			this->group_table->Name = L"group_table";
			this->group_table->Size = System::Drawing::Size(600, 321);
			this->group_table->TabIndex = 1;
			this->group_table->TabStop = false;
			this->group_table->Text = L"Table";
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
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }

};
}

