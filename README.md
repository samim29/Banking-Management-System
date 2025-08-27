# 🏦 Banking Management System (C Project)

A simple **console-based Banking Management System** built in **C** using **File Handling**.  
This project simulates basic banking operations like creating an account, depositing/withdrawing money, deleting an account, and exporting account data.

---

## 🚀 Features
- Create a new bank account (with account number, name, PIN, and balance).
- Deposit money into an account.
- Withdraw money (with balance check).
- Check account balance securely using PIN.
- List all accounts with details.
- Delete an account permanently.
- Export accounts to CSV file for external usage.
- Transaction logging (records all deposits/withdrawals in `transactions.log`).

---

## 🛠️ Technologies Used
- **C Programming**
- **File Handling** (`fopen`, `fread`, `fwrite`, `fseek`, `fprintf`)
- **Binary File Storage** for secure data management

---

## 📂 Project Structure
📁 Banking-Management-System <br/>
│-- main.c # Source code<br/>
│-- accounts.dat # Binary file storing account details<br/>
│-- transactions.log # Text log of all deposits/withdrawals<br/>
│-- accounts.csv # Exported CSV file (optional)<br/>
│-- README.md # Project documentation<br/>


---

## ⚡ How to Run
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/banking-system-c.git


Compile the project:

gcc main.c -o banking


Run the program:

./banking

📸 Sample Output
--- Banking System Menu ---
1. Create Account
2. Deposit Money
3. Withdraw Money
4. Check Balance
5. List Accounts
6. Delete Account
7. Export Accounts to CSV
0. Exit
Enter your choice:

🎯 Future Improvements

PIN masking (hide PIN while typing).

Sorting accounts by balance or account number.

Add a transaction history viewer for each account.

GUI version using C++/Java/Python.

👨‍💻 Author

Developed by Sk Samim Ali
B.Tech CSE (AI & ML) | Passionate about Systems Programming & Fullstack Development


---

