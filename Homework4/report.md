# Homework 4

## Question 2
The one form of the insert function causes a compilation error because it makes a comparison, specifically `>`, on an instance of the templated class, and Coord does not have that defined, so the compiler doesn't allow for the use of that function on an Coord sequence. 
