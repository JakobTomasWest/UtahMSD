# Part 1 - Selecting index
## Former Employee records S1
For each situation described, specify the minimal set of additional secondary indexes you should add to the table(s) to efficiently support all the queries, and briefly explain your reasoning. If no additional indexes are needed, write "None" for your answer. Assume all databases are very large.


Create table EmployeRecords(
    eID INT Primary Key,
    StartDate DATE,
    EndDate DATE
);

|  eID (int) PK | Start Date (Date)| End Date (Date) |
|---------------|------------------|-----------------|
|               |                  |                 |

Queries
1.Find all employees that started after a certain date
2.Find all employees that started on a certain date, and worked until at least another certain date

### Answer: 
1. To support the first query, finding all the employees records who started working after a certain date we should add an index on StartDate -- INDEX(StartDate)
: CREATE INDEX idxStartDate ON EmployeeRecords (StartDate)
--> This would work because it would help us reduce the amount of pages to scan 

2. To support the second query, find all the employees who worked between to point in time we could add INDEX(StartDate, EndDate) : Create INDEX idxStartDateEndDate on EmployeeRecords(StartDate, EndDate)
--> This owuld be useful because now we could fit where conditions using StartDate = " " and EndDate >= "atleastDate" and find particular eID for those who started on the new years Onboarding


## Grades S2

Create table grades (
    studentID INT
    className VARCHAR(10),
    Grade CHAR(1),
    PRIMARY KEY (studentID, className)
)
| studentID (int) PK | className(varchar(10), PK)| Grade (char(1)) |
|--------------------|---------------------------|-----------------|
|                    |                           |                 |
1. Get all students with a grade better than 'B'
2. Get all classes where any student earned a grade worse than 'D'

### Answer:
1 and 2. A useful index would be INDEX (Grade) : Create INDEX idxGrade ON Grades(Grade), 
this would be useful for both because both are trying allow us to use a where condition on grade in order to minmize searching larger tables and just finding the specific entries where grade is > 'B' for studentIDs and where grade < 'D' for students return classeNames.

## Grades s3
Queries
1. Get all classes ordered by class name
2. Get all students who earned an 'A' in a certain class

### Answer:
1. A useful index would be INDEX (ClassName) : Create INDEX idxClassName on Grades(className),
this would be useful because we can just take the Classes without having to retrieve the other data.
2. A useful multicolumn index would be would be INDEX (className, Grade) : Create INDEX idxClassGrade ON Grades(className,Grade),
this would be useful because we can use the index to point to students who are in a certain (where className = 'something') class and got an (Grade =) 'A'. Group first by className and then secondly the rows will be sorted by Grade. 

-- The multicolumn index would work for both queries

### Chess Database s4
Queries
1.select Name from Players where Elo >=2050;
2.select Name, gID from Players join Games where pID=WhitePlayer;

### Answer:
1. A useful index would be INDEX (Elo, Name) : Create INDEX idxNameELO on Players (Elo,Name), this would be useeful because now we can ignore pid and get speed up our query by finding name where  Elo >= 2050. If we do just elo this would work too cause we would find Elos for the condition and then point to the Names that meet that condition in the original table, but since Nabil is saying that we should assume that the tables are ginormous INDEX(Elo, Name) wouldn't have to read as much.
2. A useful index would be INDEX (Games.WhitePlayer) : Create INDEX idxWhitePlayer ON Games(WhitePlayer), this would be useful for this query because when we join we can now look up where the pID is specifically related to WhitePLayers and not just finding all occurences pID for the Name in the Games Table. Now that we have WhitePlayer indexed we don't have to search the whole Games Table to find matches for the join.

### Library s5
Queries
1. select * from Inventory natural join CheckedOut;

### Answer:
1. We don't need to create an indexes because the PK's for Inventory and CheckedOut are already both serial so serial is indexed.

### Library s6
Queries
1. select * from Inventory natural join CheckedOut where CardNum=2;
2. select * from Patrons natural join CheckedOut;

### Answer:
1.  The index to speed up our query for finding specific Cardnum values would be INDEX(CheckedOut.CardNum) : Create INDEX idxCoCardNum on CheckedOut(CardNum) because even though our natural join works out we would still have to scan the whole table for CheckedOut values, but having these CardNums indexed would allow us to just match join the Serial indicies where CardNum is matched, speeding it up by now having to check every row of a large data set.
2. In order to join effectively we need to create a new index because we are assuming only the Pks are indexed. The index we need is INDEX(CheckedOut.CardNum) :Create INDEX idxCoCardNum on CheckedOut(CardNum), this would prevent us from having the scan the entire table of CheckedOut and quickly join on CardNum equalities.

### Library s7
Note: Assume the only existing indexes are the primary index on each table (despite whatever indexes are on the actual tables). Also assume that the Library has been auto-scaffolded as discussed in class.

```sql
1. var query =
  from t in db.Titles
  select new
  {
    Title = t.Title,
    Serials = from i in t.Inventory select i.Serial
  };
```
### Answer:

1. Index inventory as INDEX(Inventory.ISBN) : Create INDEX idxInventoryISBN in order to find titles where the Serial is both in the Inventory and in Titles. If ISBN is not indexed in Inventory then we would need to look through the entire table to get all the rows instead of storing the pointers that relate our tuple to the Titles. Storing the index for Inventory.ISBN allows us to quickly join on ISBNs to find our Inventory Serials for matching ISBNs in each table. I'm assuming because the auto-scaffolding that we can use the 'auto-like navigation' to connect our titles and inventory tables with the shared ISBN columns and project the serial from them but only because indexing the ISBN allow us a connection between the two classes from the navigation link.

# Part 2 - B+ Tree Index Structures

### Students table:
Assume that an int occupies 4 bytes, and a varchar(10) occupies 10 bytes.
Create table grades (
    studentID INT
    className VARCHAR(10),
    Grade CHAR(1),
    PRIMARY KEY (studentID, className)
)
| studentID (int) PK | className(varchar(10), PK)| Grade (char(1)) |
|--------------------|---------------------------|-----------------|
|                    |                           |                 |
 studentID = 4 bytes.  className = 10 bytes.      Grade = 1 byte    Total per row = 15 bytes
1. How many rows of the table can be placed into the first leaf node of the primary index before it will split?
# Answer:
Page size / Total bytes --> 4096 bytes / 15 bytes = 273 rows
273 rows can be placed into the first leaf node before we split.

2. What is the maximum number of keys stored in an internal node of the primary index? (Remember to ignore pointer space. Remember that internal nodes have a different structure than leaf nodes.)
# Answer:
We have a composite key: (studenID, className) -- internal nodes have no row data.
Total key size = 14 bytes.
Page size / Total key size --> 4096 bytes / 14 bytes = 292 keys.

THe maximum of keys we can store in an internal of the primary index is 292 keys.

3. What is the maximum number of rows in the table if the primary index has a height of 1? (A tree of height 1 has 2 levels and requires exactly one internal node)
# Answer:
If a tree has a height of 1 then has one 1 root or internal node. The root can have 292 keys or 293 children/ leaf nodes. Each leaf can hold 273 rows.
273 rows / leaf x 293 leaves = 79,989 rows

The maximum number of rows that a table can have if it has a primary index with a height of 1 is 79,989 rows!


4. What is the minimum number of rows in the table if the primary index has a height of 1? (A tree of height 1 has 2 levels). The minimum capacity of a node in a B+ tree is 50%, unless it is the only internal/leaf node. The minimum number of children of a root node is 2.
# Answer:
So the root node must have atleast 2 children. And the minimum root nodes we can have is 1 then we only care about the leaf nodes rows.
We are gonna fill the leaf nodes up to 50%.
273 rows /2 = 136.5 so ~ 137 rows per leaf node
Two leaf nodes is 137  *2.

The mininum number of rows in the table with  primary index having a height of one is 274.

5. If there is a secondary index on Grade, what is the maximum number of entries a leaf node can hold in the secondary index?
# Answer:
Since we are dealing with a seconary index we don't ignore the pointer data??
Grade = 1 byte
SO if the record pointer to child nodes is 8 bytes, then the maximum of entries a leaf node can hold is 4096 bytes / (8 +1) bytes or  455 entries of the Grade Pointer pairs in a leaf node. 


### Another table:

Assume that for some table, rows occupy 128 bytes.

1. What is the maximum number of leaf nodes in the primary index if the table contains 48 rows?
# Answer:
1 row = 128 bytes
4096 bytes per page
4096 / 128 = 32 rows per page
48 rows / 32 rows means that the maxinum of leaf nodes in the primary index is 2 leaf nodes because we are over 32 rows but under 64 rows. 
2 leaf nodes required.

2. What is the minimum number of leaf nodes in the primary index if the table contains 48 rows?
# Answer:
Each leaf node must be at least 50% capcacity or 50% full so the 32/2 would be 16 rows.
Because we need to contain 48 rows then we will need a minimum of 3 leaf nodes with each row holding 16 rows --> 48 rows / 16 rows = 3 nodes min.

