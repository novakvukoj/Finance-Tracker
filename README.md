# Finance Tracker

A comprehensive personal finance management application built with C++ and Qt framework. Track your income, expenses, and financial goals with an intuitive graphical interface.

## Features

- **Transaction Management**: Add, edit, and delete financial transactions
- **Category Organization**: Organize transactions into customizable categories
- **Goal Setting**: Set and track financial goals
- **Summary & Reports**: View summaries of your financial activity
- **Cross-Platform**: Works on Windows and other platforms supporting Qt

## Video Demonstration

Watch a full walkthrough of the Finance Tracker application here:

**[CS3307 - Deliverable 3 Project Video (full project)](https://youtu.be/zV34plTpKbc)**

## Technologies Used

- **Language**: C++
- **GUI Framework**: Qt (Qt Widgets)
- **Build System**: qmake (.pro files)
- **IDE**: Visual Studio / Qt Creator

## Installation & Setup

### Prerequisites

- Qt Framework (5.x or 6.x)
- C++ Compiler (MSVC, GCC, or Clang)
- Qt Creator (recommended) or Visual Studio with Qt plugin

### Building the Project

1. **Clone the repository**
```bash
   git clone https://github.com/Fall-2025-CS-3307/oop-assignment-2-group-132.git
   cd FinanceTracker
```

2. **Open in Qt Creator**
   - Launch Qt Creator
   - Open `code/FinanceTracker.pro`
   - Configure the project with your kit

3. **Build the project**
   - Click "Build" > "Build Project" or press `Ctrl+B`
   - Alternatively, use command line:
```bash
     cd code
     qmake FinanceTracker.pro
     make
```

4. **Run the application**
   - Click the "Run" button or press `Ctrl+R`
   - Or execute the binary from `build/` directory

### Alternative: Using Visual Studio

1. Open the `.vcxproj` file in Visual Studio
2. Ensure Qt Visual Studio Tools are installed
3. Build and run the project (F5)

## Usage

1. **Launch the Application**: Run the FinanceTracker executable
2. **Add Transactions**: 
   - Click "Add Transaction" 
   - Enter amount, category, date, and description
   - Save the transaction
3. **Manage Categories**: 
   - Navigate to Category Manager
   - Create, edit, or delete categories
4. **Set Goals**: 
   - Use Goal Manager to set financial targets
   - Track progress toward your goals
5. **View Summaries**: 
   - Access the Summary page for financial insights
   - View reports and statistics

## Key Components

### Data Manager
Handles all data-related opeartions, ensuring only one system is manipulating data.

### Transaction Manager
Handles all transaction-related operations including adding, editing, deleting, and retrieving transactions.

### Category Manager
Manages transaction categories for better organization and reporting.

### Goal Manager
Allows users to set financial goals and track progress.

### Summary Module
Generates financial summaries and reports based on transaction data.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Authors

**Novak Vukojicic**
- Email: nvukojic@uwo.com

**Muhammad Shayaan Ali**
- Email: mali484@uwo.ca

## Acknowledgments

- Qt Framework for the excellent GUI toolkit
- God
