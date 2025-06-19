# Lab 01 – Relational Schema Design
**Jakob West u0675702**

## Part 1 – English to Schema

---

### 1. Grocery store tracking quantity and price

A grocery store needs to track an inventory of products for sale. It has zero or more of each type of product for sale, and needs to track the quantity and price for each product.

**Schemas**\
Inventory [__SKU(string)__, quantity(integer), price(real)]\
Product [__SKU(string)__, name(string)]

### 2. Grocery store aisle locations

A product can be within multiple aisles of the store with speciality racks, however, a product can only be displayed in one place per aisle.

**Schemas**\
Product [__SKU(string)__, name(string)]\
Location [__SKU(string)__, aisle(integer)]

### 3. Car Dealership

A car has a: make, model, year, color, vin. A salesperson has a: name, social security number (SSN) and sales. A dealership has an inventory of cars, and Salespersons. The dealership needs to assign cars to sell for salespersons. More than one salesperson can be assigned to any given car, but a car does not necessarily have any salespeople assigned to it.

**Schemas**\
Car [__VIN(string)__, make (string),model (string), year(integer), color(string)]\
Salesperson[__SSN(integer)__, name(string), sales(integer)]\
CarAssignment [__VIN(string)__, __SSN(integer)__]


## Part 2 –  SQL Table Declarations
---
```sql

CREATE TABLE Patrons (
  Name (string),
  CardNum (integer),
  PRIMARY KEY(CardNum)
);

CREATE TABLE Phones (
    CardNum (integer),
    PhoneNum (string),
    PRIMARY KEY(CardNum, PhoneNum)
    FOREIGN KEY (CardNum) REFERENCES Patrons (CardNum),
);
CREATE TABLE CheckedOut (
    CardNum (integer),
    Serial (integer),
    PRIMARY KEY(Serial),
    FOREIGN KEY (CardNum) REFERENCES Patrons (CardNum)
    
);

CREATE TABLE Inventory(
    Serial (integer),
    ISBN(string),
    PRIMARY KEY ( Serial),
    FOREIGN KEY (ISBN) REFERENCES Titles (ISBN)
);

CREATE TABLE Titles(
    ISBN(string),
    Title(string),
    Author(string)
    PRIMARY KEY (ISBN)
);

```
## Part 3 - Fill in the Tables 
 
 
Cars:

Red Toyota Tacoma, 2008\
Green Toyota Tacoma, 1999\
White Tesla Model 3, 2018\
Blue Subaru WRX, 2016\
Red Ford F150, 2004\

Salespeople:
Arnold, trying to sell all Toyotas\
Hannah, trying to sell all red cars\
Steve, selling the Tesla

Car [__VIN(string)__, make (string), year(integer), color(string)]\
Salesperson[__SSN(integer)__, name(string), sales(integer)]\
CarAssignment [__VIN(string)__, __SSN(integer)__]


**Cars**

| VIN | make   | model  |  year |  color |
|:----|:-------|:-------|:------|:-------|
| 1A  | Toyota |Tacoma  |  2008 |  Red   |
| 2A  | Toyota |Tacoma  |  1999 |  Green |
| 3A  | Tesla  |Model 3 |  2018 |  White |
| 4A  | Subaru |WRX     |  2016 |  Blue  |
| 5A  | Ford   |F150    |  2004 |  Red   |


**Salesperson**
|   SSN   |   Name  |    Sales    |
|:--------|:--------|:------------|
|123456789|  Arnold | all toyotas |
|123456781|  Hannah | all red cars|
|123456782|  Steve  | the tesla   |

**Car Assignment**
|  SSN    |VIN |
|:--------|:---|
|123456789|1A   |     
|123456789|2A   | 
|123456781|1A   | 
|123456781|5A   | 
|123456782|3A   | 


## Part 4 - Keys and Superkeys
Consider the following instance:

| A1  | A2   | A3  |
|:----|:-----|:----|
| x   | 4.0  | q   |
| y   | 4.0  | p   |
| z   | 3.1  | p   |
| z   | 4.0  | p   |

Assume that the instance data is complete: in other words, if a set of fields is unique among all tuples, then it can be considered always unique (no new additions to the table will ever change that).

Fill in the missing entries in the table below. Some cells have been given as an example.

| Attribute Sets | Superkey? | Proper Subsets | Key?|
|----------------|-----------|----------------|-----|
| {A1}           |  No      | {}             | No |
| {A2}           |  No      | {}             | No |
| {A3}           |  No      | {}             | No |
| {A1, A2}       |  YES      | {A1}, {A2}     | YES |
| {A1, A3}       |  No      | {A1}, {A3}     | No |
| {A2, A3}       |  No      | {A2}, {A3}     | No |
| {A1, A2, A3}   |  YES      | {A1}, {A2}, {A3}, {A1, A2}, {A1, A3}, {A2, A3} | No |			


## Part 5 - Abstract Reasoning

For each statement below, indicate whether it is true or false, and briefly explain your reasoning in one or two sentences. The variables "x", "y", and "z" below refer to attributes, not values. You do not need to give a formal mathematical proof, just give a well-reasoned explanation.

If {x} is a superkey, then any set containing x is also a superkey.
    
    This is True. If {x} is a superkey, then it uniquely identifies all rows in the table. Adding any other attributes to {x} (such as {y} or {z}) will not break that uniqueness. Therefore, any set that includes {x} is also a superkey.

If {x} is a key, then any set containing x is also a key.
    
    This is false. If {x} is a key, then any set containing {x} is not a key because it would no longer be minimal. For example, if we add another attribute {y} to {x}, we get a new set {x, y} which still uniquely identifies rows — making it a superkey — but not a key, because it includes unnecessary attributes. Keys must be minimal sets that uniquely identify rows.

If {x} is a key, then {x} is also a superkey.
    
    Yes, this is true. A superkey is a set that uniquely identifies a row, and key is the most minimal set that uniquely identifies a row. A key is by definition a superkey. 

If {x, y, z} is a superkey, then one of {x}, {y}, or {z} must also be a superkey.
    
    No, this is false. If {x, y, z} is a superkey, that means the combination of all three attributes uniquely identifies each row. However, it does not mean that any individual attribute ({x}, {y}, or {z}) is a superkey on its own. Each of those attributes could have duplicate values in their columns, and only together do we know that they ensure uniqueness. So {x, y, z} can be a minimal superkey (i.e., a key), and none of its parts alone would qualify as a superkey.

If an entire schema consists of the set {x, y, z}, and if none of the proper subsets of {x, y, z} are keys, then {x, y, z} must be a key.
   
    Yes, this is true. Each table or schema needs to have a key. If it is not a subset of {x,y,z}, then {x, y, z} must be the key.

