# Lab 01 – Relational Schema Design

## Part 1 – English to Schema

---

### 1. Grocery store tracking quantity and price

A grocery store needs to track an inventory of products for sale. It has zero or more of each type of product for sale, and needs to track the quantity and price for each product.

Inventory [__SKU(string)__, quantity(integer), price(real)]\
Product [__SKU(string)__, name(string)]

### 2. Grocery store aisle locations

A product can be within multiple aisles of the store with speciality racks, however, a product can only be displayed in one place per aisle.

Product [__SKU(string)__, name(string)]\
Location [__SKU(string)__, aisle(integer)]

### 3. Car Dealership

A car has a: make, model, year, color, vin. A salesperson has a: name, social security number (SSN) and sales. A dealership has an inventory of cars, and Salespersons. The dealership needs to assign cars to sell for salespersons. More than one salesperson can be assigned to any given car, but a car does not necessarily have any salespeople assigned to it.

Car [__VIN(string)__, make (string), year(integer), color(string)]\
Salesperson[__SSN(integer)__, name(string), sales(integer)]\
CarAssignment [__VIN(string)__, __SSN(integer)__]


## Part 2 –  SQL Table Declarations
---
```sql

CREATE TABLE Patrons (
  Name (string),
  CardNum (integer),
  PRIMARY KEY(CardNum)

)

CREATE TABLE Phones (
    CardNum (integer),
    PhoneNum (integer),
    PRIMARY KEY(CardNum, PhoneNum)

)
CREATE TABLE CheckedOut (
    CardNum (integer),
    Serial (integer),
    PRIMARY KEY(Serial, Card),
    FOREIGN KEY CardNum REFERENCES Patrons (CardNum),
    FOREIGN KEY Serial REFERENCES Inventory (Serial),
    UNIQUE (Serial)
    
)

CREATE TABLE Inventory(
    Serial (integer),
    ISBN(integer),
    PRIMARY KEY ( Serial),
    FOREIGN KEY ISBN REFERENCES Titles (ISBN)
    UNIQUE( Serial, ISBN)
)

CREATE TABLE Titles(
    ISBN(integer) PRIMARY KEY,
    Title(string),
    Author(string)
    UNIQUE (Title, Author)
);

```
## Part 3 - Fill in the Tables 
 
 The dealership needs to assign cars to sell for salespersons. More than one salesperson can be assigned to any given car, but a car does not necessarily have any salespeople assigned to it.

Cars:

Red Toyota Tacoma, 2008
Green Toyota Tacoma, 1999
White Telsa Model 3, 2018
Blue Subaru WRX, 2016
Red Ford F150, 2004
Salespeople:

Arnold, trying to sell all Toyotas
Hannah, trying to sell all red cars
Steve, selling the Tesla

Car [__VIN(string)__, make (string), year(integer), color(string)]\
Salesperson[__SSN(integer)__, name(string), sales(integer)]\
CarAssignment [__VIN(string)__, __SSN(integer)__]


A car has a: make, model, year, color, vin. A salesperson has a: name, social security number (SSN) and sales.

|   VIN  |   make     |  year |  color |
|:---|:---------------|:------|:-------|
| 1  | Toyota Tacoma  |  2008 |  Red   |
| 2  | Toyota Tacoma  |  1999 |  Green |
| 3  | Telsa Model 3  |  2018 |  White |
| 4  | Subaru WRX     |  2016 |  Blue  |
| 5  | Ford F150      |  2004 |  Red   |


Salespersons
|   SSN   |   Name  |    Sales    |
|:--------|:--------|:------------|
|123456789|  Arnold | all toyotas |
|123456781|  Hannah | all red cars|
|123456782|  Steve  | the tesla   |

Car Assignment
|  SSN    |VIN |
|:--------|:---|
|123456789|1   |     
|123456789|2   | 
|123456781|1   | 
|123456781|2   | 
|123456782|3   | 


## Part 4 - Keys and Superkeys
Consider the following instance:

| A1 | A2  | A3 |
|----|-----|----|
| x  | 4.0 | q  |
| y  | 4.0 | p  |
| z  | 3.1 | q  |
| z  | 4.0 | q  |
Assume that the instance data is complete: in other words, if a set of fields is unique among all tuples, then it can be considered always unique (no new additions to the table will ever change that).

Fill in the missing entries in the table below. Some cells have been given as an example.

| Attribute Sets | Superkey? | Proper Subsets | Key?|
|----------------|-----------|----------------|-----|
| {A1}           |  N/A      | {}             | N/A |
| {A2}           |  N/A      | {}             | N/A |
| {A3}           |  N/A      | {}             | N/A |
| {A1, A2}       |  YES      | {A1}, {A2}     | YES |
| {A1, A3}       |  N/A      | {A1}, {A3}     | N/A |
| {A2, A3}       |  N/A      | {A2}, {A3}     | N/A |
| {A1, A2, A3}   |  YES      | {A1}, {A2}, {A2}, {A1, A2}, {A1, A3}, {A2, A3} | N/A |			


## Part 5 - Abstract Reasoning

For each statement below, indicate whether it is true or false, and briefly explain your reasoning in one or two sentences. The variables "x", "y", and "z" below refer to attributes, not values. You do not need to give a formal mathematical proof, just give a well-reasoned explanation.

If {x} is a superkey, then any set containing x is also a superkey.


If {x} is a key, then any set containing x is also a key.


If {x} is a key, then {x} is also a superkey.


If {x, y, z} is a superkey, then one of {x}, {y}, or {z} must also be a superkey.


If an entire schema consists of the set {x, y, z}, and if none of the proper subsets of {x, y, z} are keys, then {x, y, z} must be a key.

