@echo off
echo Compiling Library Management System - CLI Version...
cd /d "c:\Users\my pc\OneDrive\Desktop\oop  project final 2\LibrarySystem"

g++ main_cli.cpp ^
  src/core/LibrarySystem.cpp ^
  src/users/Person.cpp ^
  src/users/user.cpp ^
  src/users/admin.cpp ^
  src/resources/Resource.cpp ^
  src/resources/BudgetPickBook.cpp ^
  src/resources/ClassicShelfBook.cpp ^
  src/resources/PrimePickBook.cpp ^
  src/Membership/Membership.cpp ^
  src/Membership/NormalMembership.cpp ^
  src/Membership/ExtraMembership.cpp ^
  src/Membership/DeluxeMembership.cpp ^
  src/services/Review.cpp ^
  src/exceptions/LibraryException.cpp ^
  src/exceptions/BorrowLimitExceededException.cpp ^
  src/exceptions/InsufficientBalanceException.cpp ^
  src/exceptions/ResourceNotAvailableException.cpp ^
  src/transactions/BorrowRecord.cpp ^
  -o library.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ✓ Compilation successful! 
    echo ✓ Executable created: library.exe
    echo.
    echo To run the library system, execute: library.exe
) else (
    echo.
    echo ✗ Compilation failed. Please check the errors above.
)
pause
