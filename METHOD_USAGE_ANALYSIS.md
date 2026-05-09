# Method Usage Analysis - Library System

## Summary
- **Files Analyzed**: Resource.h/cpp, PrimePickBook.h/cpp, ClassicShelfBook.h/cpp, BudgetPickBook.h/cpp, DigitalLibrary.h/cpp
- **Codebase Scanned**: main_cli.cpp, main.cpp, LibrarySystem.cpp, user.cpp, and all resource implementations
- **Total Methods Found**: 30+ methods across all resource classes
- **Used Methods**: 16
- **Unused Methods**: 10+
- **Unused Classes**: 1 (DigitalLibrary)

---

## ✅ METHODS THAT ARE USED

### Resource.h (Base Class) - 10 methods USED

| Method | Usage Count | Where Used |
|--------|---|---|
| `getResourceID()` const | 40+ | main_cli.cpp, main.cpp, LibrarySystem.cpp, user.cpp |
| `getTitle()` const | 35+ | main_cli.cpp, main.cpp, LibrarySystem.cpp, user.cpp |
| `getAuthor()` const | 35+ | main_cli.cpp, main.cpp, LibrarySystem.cpp, user.cpp |
| `getCategory()` const | 25+ | main_cli.cpp, main.cpp, LibrarySystem.cpp |
| `getAvailability()` | 30+ | main_cli.cpp, main.cpp, LibrarySystem.cpp, user.cpp |
| `getRating()` const | 20+ | main_cli.cpp, main.cpp, LibrarySystem.cpp |
| `getBorrowCount()` const | 15+ | main.cpp, LibrarySystem.cpp |
| `getReviewScore()` const | 15+ | main.cpp, LibrarySystem.cpp |
| `getIsNewArrival()` const | 5+ | LibrarySystem.cpp (filterByNewArrivals line 309) |
| `addRating(int value)` | 10+ | main_cli.cpp (line 418), main.cpp (lines 680-683) |
| `incrementBorrowCount()` | 2 | user.cpp (line 135) |
| `updateAvailability(bool status)` | 5+ | user.cpp (lines 134, 177), LibrarySystem.cpp (line 691) |

### Derived Classes - 0 DIRECT method calls to overridden methods
- `PrimePickBook::getFineRate()` - USED INDIRECTLY
- `ClassicShelfBook::getFineRate()` - USED INDIRECTLY
- `BudgetPickBook::getFineRate()` - USED INDIRECTLY

**Where getFineRate() is called**: user.cpp line 203
```cpp
double fine = lateDays * r->getFineRate();
```
Called on Resource* pointer (polymorphic call), so actual implementations are used.

---

## ❌ METHODS THAT ARE NOT USED (UNUSED)

### PrimePickBook
| Method | Status | Notes |
|--------|--------|-------|
| `displayDetails()void` | **UNUSED** | Virtual method never called on PrimePickBook objects. The override exists but is never invoked anywhere in the codebase. |
| `getBorrowLimit() int` | **UNUSED** | Virtual method implemented but never called. Unclear what this is used for. |

### ClassicShelfBook
| Method | Status | Notes |
|--------|--------|-------|
| `displayDetails()` | **UNUSED** | Virtual method never called on ClassicShelfBook objects. |
| `getBorrowLimit() int` | **UNUSED** | Virtual method implemented but never called. |

### BudgetPickBook
| Method | Status | Notes |
|--------|--------|-------|
| `displayDetails()` | **UNUSED** | Virtual method never called on BudgetPickBook objects. |
| `getBorrowLimit() int` | **UNUSED** | Virtual method implemented but never called. |

### Resource.h (Base Class)
| Method | Status | Notes |
|--------|--------|-------|
| `getDigitalAvailable() bool` | **UNUSED** | Never referenced anywhere in the codebase. Appears to be a relic. |

### DigitalLibrary (Entire Class)
| Method | Status | Notes |
|--------|--------|-------|
| `viewOnline(int resourceID)` | **UNUSED** | Class never instantiated. Only exists in commented-out code (main.cpp line 444). |
| `downloadPDF(int resourceID)` | **UNUSED** | Class never instantiated. |
| `trackAccessCount()` | **UNUSED** | Class never instantiated. |

---

## 📦 CLASS INSTANTIATION STATUS

### ✅ Classes THAT ARE INSTANTIATED

| Class | Instantiated | Count | Where |
|-------|---|---|---|
| **PrimePickBook** | ✅ YES | 6+ instances | main.cpp (lines 435-436), main_cli.cpp (line 782), LibrarySystem.cpp (line 556) |
| **ClassicShelfBook** | ✅ YES | 6+ instances | main.cpp (lines 438-439), main_cli.cpp (line 784), LibrarySystem.cpp (line 558) |
| **BudgetPickBook** | ✅ YES | 6+ instances | main.cpp (lines 441-442), main_cli.cpp (line 786), LibrarySystem.cpp (line 560) |

### ❌ Classes THAT ARE NOT INSTANTIATED

| Class | Status | Issue | Location |
|-------|--------|-------|----------|
| **DigitalLibrary** | ❌ NOT USED | Never instantiated anywhere in active code. Only appears in commented-out test code. | main.cpp line 444: `// DigitalLibrary ebook1 = new DigitalLibrary(...);` |

---

## 🔍 DETAILED FINDINGS

### 1. displayDetails() - Defined but Never Called
All three resource subclasses implement `displayDetails()` but it's never called:
```cpp
// In PrimePickBook.cpp (line 7)
void PrimePickBook::displayDetails() { ... }

// In ClassicShelfBook.cpp (line 7)  
void ClassicShelfBook::displayDetails() { ... }

// In BudgetPickBook.cpp (line 7)
void BudgetPickBook::displayDetails() { ... }
```

**Search Result**: Only matches found are the method definitions themselves, not a single call site.

### 2. getBorrowLimit() - Defined but Never Directly Called
All three resource subclasses implement this:
```cpp
int PrimePickBook::getBorrowLimit() { return 5; }
int ClassicShelfBook::getBorrowLimit() { return 2; }
int BudgetPickBook::getBorrowLimit() { return 3; }
```

**Search Result**: No call sites found. These values appear to be unused.

### 3. getFineRate() - ACTIVELY USED
This is the ONLY virtual method from subclasses that's actively called:
- Location: [user.cpp](user.cpp#L203) line 203
```cpp
double fine = lateDays * r->getFineRate();
```
Called on a `Resource*` pointer, polymorphically invoking the correct subclass implementation:
- PrimePickBook returns 5.0
- ClassicShelfBook returns 3.0
- BudgetPickBook returns 2.0

### 4. getDigitalAvailable() - Completely Unused
This method is defined in Resource.h but never referenced:
```cpp
bool Resource::getDigitalAvailable() const { return digitalAvailable; }
```
No search results for this method being called anywhere.

### 5. DigitalLibrary Class - Complete Dead Code
The entire `DigitalLibrary` class is unused:
- **No instantiations** in active code (only commented out)
- **No method calls** to any of its methods
- **Public methods**:
  - `viewOnline(int resourceID)` - outputs to console
  - `downloadPDF(int resourceID)` - outputs to console
  - `trackAccessCount()` - outputs to console

The only reference in active code is as a comment:
```cpp
// DigitalLibrary ebook1 = new DigitalLibrary(2001, "Digital Programming", "Robert Martin", "Technology");
```

---

## 📊 USAGE BREAKDOWN BY FILE

### main_cli.cpp
- ✅ Uses: getResourceID, getTitle, getAuthor, getCategory, getAvailability, getRating, addRating
- ❌ Doesn't use: displayDetails, getBorrowLimit, getDigitalAvailable, DigitalLibrary

### main.cpp
- ✅ Uses: getResourceID, getTitle, getAuthor, getCategory, getAvailability, getRating, getBorrowCount, getReviewScore, addRating
- ❌ Doesn't use: displayDetails, getBorrowLimit, getDigitalAvailable, any DigitalLibrary methods

### LibrarySystem.cpp
- ✅ Uses: getResourceID, getTitle, getAuthor, getCategory, getAvailability, getRating, getBorrowCount, getReviewScore, getIsNewArrival, updateAvailability
- ❌ Doesn't use: displayDetails, getBorrowLimit, getDigitalAvailable

### user.cpp
- ✅ Uses: getAvailability, updateAvailability, getCategory, getResourceID, getTitle, getFineRate, incrementBorrowCount
- ❌ Doesn't use: displayDetails, getBorrowLimit, getDigitalAvailable

---

## 🎯 RECOMMENDATIONS

### High Priority - Remove Dead Code
1. **Delete DigitalLibrary class entirely** - It's not used and not integrated
   - Delete: DigitalLibrary.h, DigitalLibrary.cpp
   - Remove include from files that include it

2. **Remove displayDetails() override implementations** - If polymorphic display is needed, it's not implemented
   - Consider: Either remove these methods or implement a feature that uses them
   - Current implementations in PrimePickBook.cpp, ClassicShelfBook.cpp, BudgetPickBook.cpp lines 7-10

3. **Remove getBorrowLimit() implementations** - No logic uses these values
   - Appears to be vestigial design that was never completed
   - All three subclasses implement this but it's never called

### Medium Priority - Clean Up
1. **Remove getDigitalAvailable() from Resource.h** - Completely unused
   - Check if `digitalAvailable` member variable is used anywhere (it's not)
   - This appears to be incomplete digital library support

2. **Clarify getFineRate() usage** - Currently only used indirectly
   - Document that this is the PRIMARY polymorphic method being used
   - The design works correctly here

### Low Priority - Optimization
1. Consider if `getReviewScore()` and `getRating()` should both exist (they return same thing: `rating` member)
2. Both methods are used, so keep them

---

## 📈 Code Statistics

| Metric | Count |
|--------|-------|
| Total methods in Resource classes | 30+ |
| Methods actually used | 16 |
| Methods never called | 10+ |
| Classes instantiated | 3 (PrimePickBook, ClassicShelfBook, BudgetPickBook) |
| Classes NOT instantiated | 1 (DigitalLibrary) |
| Unused classes by percentage | 33% |
| Method coverage | ~53% |

